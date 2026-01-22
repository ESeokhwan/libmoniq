#include "libmoniq/adaptor/latency_monitoring_message_adaptor.h"
#include "libmoniq/include/json.hpp"

#include <chrono>
#include <random>
#include <sstream>

using json = nlohmann::json;

namespace moniq {
namespace adaptor {

std::string JsonBasedLatencyMonitoringMessageAdaptor::generate(std::string messageId) {
    auto now = std::chrono::system_clock::now();
    auto nanosec = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());
    double millsec = nanosec.count() / 1e6;
    return generate(messageId, millsec);
}

std::string JsonBasedLatencyMonitoringMessageAdaptor::generate(std::string messageId, double requested_at) {
    json messageJson = {
        {ID_KEY_, messageId},
        {REQUESTED_AT_KEY, requested_at},
        {PAYLOAD_KEY_, get_random_payload()}
    };
    return messageJson.dump();
}

std::string JsonBasedLatencyMonitoringMessageAdaptor::generate(std::string messageId, double requested_at, std::map<std::string, std::string> oth_kvs) {
    json messageJson = {
        {ID_KEY_, messageId},
        {REQUESTED_AT_KEY, requested_at},
        {PAYLOAD_KEY_, get_random_payload()}
    };
    for (const auto& [key, value] : oth_kvs) {
        messageJson[key] = value;
    }
    return messageJson.dump();
}

std::string JsonBasedLatencyMonitoringMessageAdaptor::extract_content(const std::string& message) const {
    try {
        json messageJson = json::parse(message);
        if (messageJson.contains(ID_KEY_)) return messageJson[ID_KEY_];
    } catch (...) {
        return "";
    }
    return "";
}

double JsonBasedLatencyMonitoringMessageAdaptor::extract_requested_at(const std::string& message) const {
    try {
        json messageJson = json::parse(message);
        if (messageJson.contains(REQUESTED_AT_KEY)) {
            if (messageJson[REQUESTED_AT_KEY].is_number()) return messageJson[REQUESTED_AT_KEY].get<int64_t>();
            if (messageJson[REQUESTED_AT_KEY].is_string()) return std::stoll(messageJson[REQUESTED_AT_KEY].get<std::string>());
        }
    } catch (...) {
        return -1;
    }
    return -1;
}

std::string JsonBasedLatencyMonitoringMessageAdaptor::extract_other_kvs(const std::string& message, const std::string& key) const {
    try {
        json messageJson = json::parse(message);
        if (messageJson.contains(key)) return messageJson[key];
    } catch (...) {
        return "";
    }
    return "";
}

JsonBasedLatencyMonitoringMessageGenerator::JsonBasedLatencyMonitoringMessageGenerator(int payload_size, int pre_indices_size)
    : JsonBasedLatencyMonitoringMessageAdaptor(), payload_size_(payload_size), cur_idx_(0) {
    init_(pre_indices_size);
}

void JsonBasedLatencyMonitoringMessageGenerator::init_(int pre_indices_size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, static_cast<int>(PAYLOAD_CHARACTERS_.size()) - 1);

    pre_generated_indices_.resize(pre_indices_size);
    cur_idx_ = 0;
    for (int& idx : pre_generated_indices_) {
        idx = dist(gen);
    }
}

std::string JsonBasedLatencyMonitoringMessageGenerator::get_random_payload() {
    std::ostringstream payload;
    for (int i = 0; i < payload_size_; i++) {
        int pre_generated_size = static_cast<int>(pre_generated_indices_.size());
        if (cur_idx_ >= pre_generated_size * MAX_CUR_IDX_MULTIPLIER_) {
            cur_idx_ = 0;
        }
        char random_char = PAYLOAD_CHARACTERS_[pre_generated_indices_[cur_idx_ % pre_generated_size]];
        payload << random_char;
        cur_idx_ += 1;
    }
    return payload.str();
}

}
}