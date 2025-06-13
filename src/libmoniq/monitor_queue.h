#pragma once

#include "libmoniq/monitor_log.h"

#include <queue>
#include <mutex>
#include <atomic>
#include <memory>

namespace monitor {

class MonitorQueue {
private:
    std::queue<std::unique_ptr<MonitorLog>> queue_;
    mutable std::mutex mutex_;
    std::atomic<int> size_{0};

public:
    MonitorQueue();

    bool enqueue(std::unique_ptr<MonitorLog> log);
    std::unique_ptr<MonitorLog> dequeue();

    int size() const;
    bool is_empty() const;
};

}
