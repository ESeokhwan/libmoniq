#pragma once

#include "libmoniq/monitor_log.h"

#include <optional>

namespace moniq {

class ILatencyMonitorLog: public IMonitorLog {
public:
    virtual std::string get_content() const = 0;
    virtual double get_requested_at() const = 0;
    virtual double get_responded_at() const = 0;
    virtual double get_latency() const = 0;

    virtual ~ILatencyMonitorLog() = default;
};

class JsonBasedLatencyMonitorLog: public ILatencyMonitorLog {
private:
    std::string raw_data_;
    std::string status_;
    double responded_at_;

    std::optional<std::string> extracted_content_;
    std::optional<double> extracted_requested_at_;

public:
    JsonBasedLatencyMonitorLog(const std::string& raw_data, const std::string& status, double responded_at);
    ~JsonBasedLatencyMonitorLog() = default;

    std::vector<std::string> get_headers() const override;
    std::vector<std::string> get_values() const override;
    void preprocess() override;

    std::string get_raw_data() const { return raw_data_; }
    std::string get_status() const { return status_; }
    std::string get_content() const override;
    double get_requested_at() const override;
    double get_responded_at() const override;
    double get_latency() const override;
};

} // namespace monitor