#pragma once

#include "libmoniq/monitor_log.h"
#include "libmoniq/monitor_queue.h"
#include "libmoniq/writer/write_strategy/monitor_log_write_strategy.h"
#include "libmoniq/util/message_adaptor.h"

#include <atomic>
#include <condition_variable>
#include <mutex>

namespace monitor {
namespace writer {

class MonitorLogWriter {
public:
    MonitorLogWriter(
        MonitorQueue& monitor_queue,
        IMonitorLogWriteStrategy& write_strategy,
        int batch_size);

    void graceful_shutdown();
    void notify_if_needed();
    void run();

private:
    void synced_wait();
    void synced_notify();
    void try_flush_batch();
    void flush_batch();

    MonitorQueue& monitor_queue_;
    IMonitorLogWriteStrategy& write_strategy_;
    int batch_size_;
    std::atomic<bool> terminated_{false};
    int curWrittenCnt_ = 0;

    std::mutex mtx_;
    std::condition_variable cv_;
};

}
}