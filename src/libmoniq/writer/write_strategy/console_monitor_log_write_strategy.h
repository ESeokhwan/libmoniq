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
    bool scrapable_;

public:
    ConsoleMonitorLogWriteStrategy(bool scrapable);

    ~ConsoleMonitorLogWriteStrategy() = default;

    void write(std::unique_ptr<IMonitorLog> log_pq);

    bool commit();
};

}
}