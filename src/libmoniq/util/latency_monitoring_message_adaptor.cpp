#include "libmoniq/util/latency_monitoring_message_adaptor.h"
#include "libmoniq/include/json.hpp"

#include <chrono>
#include <random>

using json = nlohmann::json;

namespace moniq {
namespace util {

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

std::string JsonBasedLatencyMonitoringMessageAdaptor::extract_message_id(const std::string& message) {
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

const int MAX_CUR_IDX_MULTIPLIER = 10;

const std::string JsonBasedLatencyMonitoringMessageGenerator::payload_characters =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

JsonBasedLatencyMonitoringMessageGenerator::JsonBasedLatencyMonitoringMessageGenerator(int payload_size, int pre_indices_size)
    : JsonBasedLatencyMonitoringMessageAdaptor(), payload_size_(payload_size), cur_idx_(0) {
    init_(pre_indices_size);
}

void JsonBasedLatencyMonitoringMessageGenerator::init_(int pre_indices_size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, static_cast<int>(payload_characters.size()) - 1);

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
        if (cur_idx_ >= pre_generated_size * MAX_CUR_IDX_MULTIPLIER) {
            cur_idx_ = 0;
        }
        char random_char = payload_characters[pre_generated_indices_[cur_idx_ % pre_generated_size]];
        payload << random_char;
        cur_idx_ += 1;
    }
    return payload.str();
}

}
}