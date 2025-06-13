#include "libmoniq/writer/write_strategy/console_monitor_log_write_strategy.h"

#include <ctime>
#include <iomanip>
#include <sstream>

namespace monitor {
namespace writer {

std::string prettier_timestamp(int64_t timestamp);
std::string prettier_timestamp_nano(int64_t timestamp);

ConsoleMonitorLogWriteStrategy::ConsoleMonitorLogWriteStrategy(
    util::IMessageAdaptor& message_adaptor, 
    bool scrapable,
    bool need_prettier,
    bool need_timestamp)
    : message_adaptor_(message_adaptor),
      scrapable_(scrapable),
      need_prettier_(need_prettier),
      need_timestamp_nano_(need_timestamp) 
{}

void ConsoleMonitorLogWriteStrategy::write(std::unique_ptr<MonitorLog> log_pq) {
    if (log_pq == nullptr) {
        return;
    }
    std::string message_id = message_adaptor_.extract_message_id(log_pq->get_id());
    std::string timestamp_str = prettier_timestamp(log_pq->get_timestamp());
    std::string timestamp_nano_str = prettier_timestamp_nano(log_pq->get_timestamp_nano());
    
    std::string output = "";
    if (scrapable_) {
        output += get_request_type_string(log_pq->get_type()) + ", ";
        output += message_id + ", ";
        output += timestamp_str + ", ";
        if (need_timestamp_nano_) output += timestamp_nano_str + ", ";
        output += get_request_state_string(log_pq->get_state());
        printf("%s\n", output.c_str());
        return;
    }

    output += "MonitorLog -- ";
    output += "Type: " + get_request_type_string(log_pq->get_type()) + ", ";
    output += "Id: " + message_id + ", ";
    output += "Timestamp: " + timestamp_str + ", ";
    if (need_timestamp_nano_) output += "TimestampNano: " + timestamp_nano_str + ", ";
    output += "State: " + get_request_state_string(log_pq->get_state()) + ", ";
    printf("%s\n", output.c_str());
}

bool ConsoleMonitorLogWriteStrategy::commit() {
    return true;
}

std::string ConsoleMonitorLogWriteStrategy::prettier_timestamp(int64_t timestamp) {
    if (need_prettier_) {
        std::time_t t = static_cast<std::time_t>(timestamp / 1000);
        int millis = static_cast<int>(timestamp % 1000);

        std::tm* tm_time = std::gmtime(&t);
        std::ostringstream oss;
        oss << std::put_time(tm_time, "%Y-%m-%dT%H:%M:%S")
            << '.' << std::setw(3) << std::setfill('0') << millis;
        return oss.str();
    } else {
        return std::to_string(timestamp);
    }
}

std::string ConsoleMonitorLogWriteStrategy::prettier_timestamp_nano(int64_t timestampNano) {
    if (need_prettier_) {
        return std::to_string(timestampNano % 1'000'000);
    } else {
        return std::to_string(timestampNano);
    }
}

}
}