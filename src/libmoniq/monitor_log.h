#pragma once

#include <string>

namespace monitor {

enum class RequestType {
    PRODUCE,
    CONSUME
};

inline std::string get_request_type_string(RequestType type) {
    switch (type) {
        case RequestType::PRODUCE:
            return "PRODUCE";
        case RequestType::CONSUME:
            return "CONSUME";
        default:
            return "UNKNOWN";
    }
}

enum class State {
    REQUESTED,
    RESPONDED
};

inline std::string get_request_state_string(State state) {
    switch (state) {
        case State::REQUESTED:
            return "REQUESTED";
        case State::RESPONDED:
            return "RESPONDED";
        default:
            return "UNKNOWN";
    }
}

class MonitorLog {
private:
    RequestType type_;
    std::string id_;
    State state_;
    int64_t timestamp_;
    int64_t timestamp_nano_;

public:
    MonitorLog(RequestType type, const std::string& id, State state, int64_t timestamp, int64_t timestamp_nano): type_(type), id_(id), state_(state), timestamp_(timestamp), timestamp_nano_(timestamp_nano) {}

    RequestType get_type() const {
        return type_;
    }

    std::string get_id() const {
        return id_;
    }

    State get_state() const {
        return state_;
    }

    int64_t get_timestamp() const {
        return timestamp_;
    }

    int64_t get_timestamp_nano() const {
        return timestamp_nano_;
    }

    bool equals(const MonitorLog& other) const {
        return type_ == other.type_ &&
            id_ == other.id_ &&
            state_ == other.state_;
    }
};
}