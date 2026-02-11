#pragma once

#include <string>
#include <vector>

namespace moniq {

class IMonitorLog {
public:
    virtual std::vector<std::string> get_headers() const = 0;
    virtual std::vector<std::string> get_values() const = 0;
    virtual void preprocess() = 0;

    virtual ~IMonitorLog() = default;
};


class MonitorLog: public IMonitorLog {
private:
    std::string content_;
    std::string state_;
    int64_t timestamp_;

public:
    MonitorLog(std::string content, std::string state, int64_t timestamp): content_(content), state_(state), timestamp_(timestamp) {}

    std::vector<std::string> get_headers() const override {
        return {
            "Content",
            "State",
            "Timestamp"
        };
    }

    std::vector<std::string> get_values() const override {
        return { content_, state_, std::to_string(timestamp_) };
    }

    void preprocess() override {}

    virtual std::string get_content() const {
        return content_;
    }

    virtual std::string get_state() const {
        return state_;
    }

    virtual int64_t get_timestamp() const {
        return timestamp_;
    }

    bool equals(const MonitorLog& other) const {
        return content_ == other.content_ &&
            state_ == other.state_;
    }
};

}