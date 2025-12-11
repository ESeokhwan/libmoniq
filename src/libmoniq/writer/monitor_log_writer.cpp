#include "libmoniq/writer/monitor_log_writer.h"

#include <thread>

namespace moniq {
namespace writer {

MonitorLogWriter::MonitorLogWriter(
    MonitorQueue& monitor_queue,
    IMonitorLogWriteStrategy& write_strategy,
    int batch_size,
    int timeout)
    : monitor_queue_(monitor_queue),
      write_strategy_(write_strategy),
      batch_size_(batch_size),
      timeout_(timeout)
{}

void MonitorLogWriter::run() {
    while (!(terminated_.load(std::memory_order_relaxed) && monitor_queue_.is_empty())) {
        synced_wait();
        for (int i = 0; i < std::max(1, batch_size_); i++) {
            std::unique_ptr<IMonitorLog> log_qp = monitor_queue_.dequeue();
            if (log_qp == nullptr) break;
            log_qp->preprocess();
            write_strategy_.write(std::move(log_qp));
        }
        write_strategy_.commit();
    }
}

void MonitorLogWriter::notify_if_needed() {
    if (is_batch_full() || direct_write_.load(std::memory_order_relaxed)) {
        synced_notify();
    }
}

void MonitorLogWriter::graceful_shutdown() {
    terminated_.store(true, std::memory_order_relaxed);
    synced_notify();
}

void MonitorLogWriter::enable_direct_write() {
    direct_write_.store(true, std::memory_order_relaxed);
    synced_notify();
}

void MonitorLogWriter::disable_direct_write() {
    direct_write_.store(false, std::memory_order_relaxed);
}

void MonitorLogWriter::synced_wait() {
    auto predicate = [this] {
        return is_batch_full()
            || terminated_.load(std::memory_order_relaxed)
            || (direct_write_.load(std::memory_order_relaxed) && !monitor_queue_.is_empty());
    };

    std::unique_lock<std::mutex> lock(mtx_);
    if (timeout_ > 0) cv_.wait_for(lock, std::chrono::seconds(timeout_), predicate);
    else cv_.wait(lock, predicate);
}

void MonitorLogWriter::synced_notify() {
    cv_.notify_one();
}

bool MonitorLogWriter::is_batch_full() {
    if (batch_size_ < 0) return false; // when batch size is less than zero, it is considered as infinite.
    return monitor_queue_.size() >= batch_size_;
}

} // namespace writer
} // namespace monitor
