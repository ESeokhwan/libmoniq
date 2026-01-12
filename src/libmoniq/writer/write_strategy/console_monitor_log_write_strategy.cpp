#include "libmoniq/writer/write_strategy/console_monitor_log_write_strategy.h"

#include <ctime>

namespace moniq {
namespace writer {

ConsoleMonitorLogWriteStrategy::ConsoleMonitorLogWriteStrategy(bool scrapable): scrapable_(scrapable) { }

void ConsoleMonitorLogWriteStrategy::write(std::unique_ptr<IMonitorLog> log_pq) {
    if (log_pq == nullptr) return;

    std::string output = "";
    if (scrapable_) {
        for (auto values: log_pq->get_values()) {
            output += values + ", ";
        }
        printf("%s\n", output.c_str());
        return;
    }

    output += "MonitorLog -- ";
    for (size_t i = 0; i < log_pq->get_values().size(); i++) {
        std::string header = log_pq->get_headers().size() > i ? log_pq->get_headers()[i] : "";
        output += header + ": " + log_pq->get_values()[i] + ", ";
    }
    printf("%s\n", output.c_str());
}

bool ConsoleMonitorLogWriteStrategy::commit() {
    return true;
}

}
}