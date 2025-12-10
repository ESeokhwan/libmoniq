#include "libmoniq/latency_monitor_log.h"
#include "libmoniq/exception/common_exceptions.h"
#include "libmoniq/util/latency_monitoring_message_adaptor.h"

namespace moniq {

JsonBasedLatencyMonitorLog::JsonBasedLatencyMonitorLog(
    const std::string& raw_data, const std::string& status, double responded_at
): raw_data_(raw_data), status_(status), responded_at_(responded_at),
    extracted_content_(std::nullopt), extracted_requested_at_(std::nullopt) {}

std::vector<std::string> JsonBasedLatencyMonitorLog::get_headers() const {
    return { "Content", "Status", "RequestedAt", "RespondedAt", "Latency" };
}

std::vector<std::string> JsonBasedLatencyMonitorLog::get_values() const {
    if (!extracted_content_.has_value() || !extracted_requested_at_.has_value()) throw NotProcessedException();
    return {
        extracted_content_.value(),
        status_,
        std::to_string(extracted_requested_at_.value()),
        std::to_string(responded_at_),
        std::to_string(get_latency())
    };
}

void JsonBasedLatencyMonitorLog::preprocess() {
    util::ExtractOnlyJsonBasedLatencyMonitoringMessageAdaptor message_adaptor;
    extracted_content_ = message_adaptor.extract_content(raw_data_);
    extracted_requested_at_ = message_adaptor.extract_requested_at(raw_data_);
}

std::string JsonBasedLatencyMonitorLog::get_content() const {
    if (!extracted_content_.has_value()) throw NotProcessedException();
    return extracted_content_.value();
}

double JsonBasedLatencyMonitorLog::get_requested_at() const {
    if (!extracted_requested_at_.has_value()) throw NotProcessedException();
    return extracted_requested_at_.value();
}

double JsonBasedLatencyMonitorLog::get_responded_at() const {
    return responded_at_;
}

double JsonBasedLatencyMonitorLog::get_latency() const {
    return responded_at_ - get_requested_at();
}

} // namespace monitor