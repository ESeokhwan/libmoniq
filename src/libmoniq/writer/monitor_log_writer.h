#pragma once

#include "libmoniq/monitor_queue.h"
#include "libmoniq/writer/write_strategy/monitor_log_write_strategy.h"

#include <atomic>
#include <condition_variable>
#include <mutex>

namespace moniq {
namespace writer {

class MonitorLogWriter {
public:
    MonitorLogWriter(
        std::shared_ptr<MonitorQueue> monitor_queue,
        std::shared_ptr<IMonitorLogWriteStrategy> write_strategy,
        int batch_size,
        int timeout
    );

    MonitorLogWriter(
        std::shared_ptr<MonitorQueue> monitor_queue,
        std::shared_ptr<IMonitorLogWriteStrategy> write_strategy,
        int batch_size,
        int timeout,
        int worker_cnt
    );

    void run();
    void notify_if_needed();
    void graceful_shutdown();

private:
    void process_write(int todo_cnt);
    void synced_wait();
    void synced_notify();
    bool is_batch_full();

    std::shared_ptr<MonitorQueue> monitor_queue_;
    std::shared_ptr<IMonitorLogWriteStrategy> write_strategy_;
    int batch_size_;
    int timeout_;
    int worker_cnt_;

    std::atomic<bool> terminated_{false};
    std::atomic<int> processed_{0};

    std::mutex mtx_;
    std::condition_variable cv_;

    std::mutex write_mtx_;
};

}
}