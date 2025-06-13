#pragma once

#include "libmoniq/monitor_log.h"

#include <memory>

namespace monitor {
namespace writer {

class IMonitorLogWriteStrategy {
public:
    virtual void write(std::unique_ptr<MonitorLog> log_pq) = 0;

    virtual bool commit() = 0;
};

}
}