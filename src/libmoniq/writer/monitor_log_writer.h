#pragma once

#include "libmoniq/monitor_log.h"
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
        MonitorQueue& monitor_queue,
        IMonitorLogWriteStrategy& write_strategy,
        int batch_size,
        int timeout
    );

    void run();
    void notify_if_needed();
    void graceful_shutdown();

private:
    void synced_wait();
    void synced_notify();
    bool is_batch_full();

    MonitorQueue& monitor_queue_;
    IMonitorLogWriteStrategy& write_strategy_;
    int batch_size_;
    int timeout_;

    std::atomic<bool> terminated_{false};

    std::mutex mtx_;
    std::condition_variable cv_;
};

}
}