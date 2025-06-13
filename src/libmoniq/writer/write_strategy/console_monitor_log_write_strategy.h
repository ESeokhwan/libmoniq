#pragma once

#include "libmoniq/monitor_log.h"
#include "libmoniq/writer/write_strategy/monitor_log_write_strategy.h"
#include "libmoniq/util/message_adaptor.h"

#include <memory>
#include <string>

namespace monitor {
namespace writer {

class ConsoleMonitorLogWriteStrategy : public IMonitorLogWriteStrategy {
private:
    util::IMessageAdaptor& message_adaptor_;
    bool scrapable_;
    bool need_prettier_;
    bool need_timestamp_nano_;

    std::string prettier_timestamp(int64_t timestamp);
    std::string prettier_timestamp_nano(int64_t timestamp);

public:
    ConsoleMonitorLogWriteStrategy(util::IMessageAdaptor& message_adaptor, 
                                   bool scrapable,
                                   bool need_prettier,
                                   bool need_timestamp_nano);

    ~ConsoleMonitorLogWriteStrategy() = default;

    void write(std::unique_ptr<MonitorLog> log_pq);

    bool commit();
};

}
}