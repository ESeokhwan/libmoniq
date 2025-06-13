#include "libmoniq/writer/monitor_log_writer.h"

#include <thread>

namespace monitor {
namespace writer {

MonitorLogWriter::MonitorLogWriter(
    MonitorQueue& monitor_queue,
    IMonitorLogWriteStrategy& write_strategy,
    int batch_size)
    : monitor_queue_(monitor_queue),
      write_strategy_(write_strategy),
      batch_size_(batch_size)
{}

void MonitorLogWriter::graceful_shutdown() {
    terminated_.store(true, std::memory_order_relaxed);
    synced_notify();
}

void MonitorLogWriter::synced_wait() {
    std::unique_lock<std::mutex> lock(mtx_);
    cv_.wait(lock, [this] {
        return terminated_.load(std::memory_order_relaxed) || monitor_queue_.size() >= batch_size_;
    });
}

void MonitorLogWriter::synced_notify() {
    cv_.notify_one();
}

void MonitorLogWriter::notify_if_needed() {
    if (monitor_queue_.size() >= batch_size_) {
        synced_notify();
    }
}

void MonitorLogWriter::run() {
    while (!(terminated_.load(std::memory_order_relaxed) && monitor_queue_.is_empty())) {
        if (!terminated_.load(std::memory_order_relaxed) && monitor_queue_.is_empty()) {
            synced_wait();
        }
        std::unique_ptr<MonitorLog> log_qp = monitor_queue_.dequeue();
        if (log_qp != nullptr) {
            write_strategy_.write(std::move(log_qp));
            curWrittenCnt_ += 1;
        }
        try_flush_batch();
    }
    flush_batch();
}

void MonitorLogWriter::try_flush_batch() {
    if (curWrittenCnt_ >= batch_size_) {
        flush_batch();
        if (!terminated_.load(std::memory_order_relaxed) && monitor_queue_.size() < batch_size_) {
            synced_wait();
        }
    }
}

void MonitorLogWriter::flush_batch() {
    write_strategy_.commit();
    curWrittenCnt_ = 0;
}

} // namespace writer
} // namespace monitor
