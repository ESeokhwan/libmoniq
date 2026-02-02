#include "libmoniq/writer/monitor_log_writer.h"
#include <atomic>
#include <memory>
#include <thread>

namespace moniq {
namespace writer {

MonitorLogWriter::MonitorLogWriter(
    std::shared_ptr<MonitorQueue> monitor_queue,
    std::shared_ptr<IMonitorLogWriteStrategy> write_strategy,
    int batch_size,
    int timeout)
    : MonitorLogWriter(monitor_queue, write_strategy, batch_size, timeout, 1)
{}

MonitorLogWriter::MonitorLogWriter(
    std::shared_ptr<MonitorQueue> monitor_queue,
    std::shared_ptr<IMonitorLogWriteStrategy> write_strategy,
    int batch_size,
    int timeout,
    int worker_cnt)
    : monitor_queue_(monitor_queue),
      write_strategy_(write_strategy),
      batch_size_(batch_size),
      timeout_(timeout), 
      worker_cnt_(worker_cnt)
{}

void MonitorLogWriter::run() {
    while (!(terminated_.load(std::memory_order_relaxed) && monitor_queue_->is_empty())) {
        synced_wait();
        int processed_batch_size = batch_size_ < 0 ? monitor_queue_->size() : batch_size_;
        if (processed_batch_size == 0) continue;

        processed_.store(0, std::memory_order_relaxed);
        std::vector<std::thread> workers;
        for (int i = 0; i < worker_cnt_; i++) {
            workers.emplace_back(std::thread(&MonitorLogWriter::process_write, this, processed_batch_size));
        }
        for (auto& worker : workers) worker.join();
        write_strategy_->commit();
    }
}

void MonitorLogWriter::notify_if_needed() {
    if (is_batch_full()) {
        synced_notify();
    }
}

void MonitorLogWriter::graceful_shutdown() {
    terminated_.store(true, std::memory_order_relaxed);
    synced_notify();
}

void MonitorLogWriter::process_write(int todo_cnt) {
    int cur_processed_ = processed_.fetch_add(1, std::memory_order_relaxed);
    while (cur_processed_ < todo_cnt) {
        try {
            std::unique_ptr<IMonitorLog> log_qp = monitor_queue_->dequeue();
            if (log_qp == nullptr) {
                processed_.store(todo_cnt, std::memory_order_relaxed);
                break;
            }
            log_qp->preprocess();
            {
                std::lock_guard<std::mutex> lock(write_mtx_);
                write_strategy_->write(std::move(log_qp));
            }
        } catch (const std::exception& e) {}
        cur_processed_ = processed_.fetch_add(1, std::memory_order_relaxed);
    }
}

void MonitorLogWriter::synced_wait() {
    auto predicate = [this] {
        return is_batch_full()
            || terminated_.load(std::memory_order_relaxed);
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
    return monitor_queue_->size() >= batch_size_;
}

} // namespace writer
} // namespace monitor
