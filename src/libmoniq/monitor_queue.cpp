#include "libmoniq/monitor_queue.h"

namespace monitor {

MonitorQueue::MonitorQueue() = default;


bool MonitorQueue::enqueue(std::unique_ptr<MonitorLog> log) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(std::move(log));
    size_.fetch_add(1, std::memory_order_relaxed);
    return true;
}


std::unique_ptr<MonitorLog> MonitorQueue::dequeue() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (queue_.empty()) {
        return nullptr;
    }
    auto outlog = std::move(queue_.front());
    queue_.pop();
    size_.fetch_sub(1, std::memory_order_relaxed);
    return outlog;
}


int MonitorQueue::size() const {
    return size_.load(std::memory_order_relaxed);
}


bool MonitorQueue::is_empty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
}

}