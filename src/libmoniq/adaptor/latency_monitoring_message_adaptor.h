#pragma once

#include "libmoniq/exception/common_exceptions.h"
#include "libmoniq/adaptor/message_adaptor.h"

#include <vector>
#include <map>

namespace moniq {
namespace adaptor {

class ILatencyMonitoringMessageAdaptor: public IMessageAdaptor {
public:
    using IMessageAdaptor::generate;

    virtual std::string generate(std::string messageId, double requested_at) = 0;

    virtual double extract_requested_at(const std::string& message) const = 0;
};


class JsonBasedLatencyMonitoringMessageAdaptor: public ILatencyMonitoringMessageAdaptor {
protected:
    static constexpr std::string_view ID_KEY_ = "id";
    static constexpr std::string_view REQUESTED_AT_KEY = "requested_at";
    static constexpr std::string_view PAYLOAD_KEY_ = "payload";

    virtual std::string get_random_payload() = 0;

public:
    std::string generate(std::string message_id) override;

    std::string generate(std::string messageId, double requested_at) override;

    virtual std::string generate(std::string messageId, double requested_at, std::map<std::string, std::string> oth_kvs);

    std::string extract_content(const std::string& message) const override;

    double extract_requested_at(const std::string& message) const override;

    virtual std::string extract_other_kvs(const std::string& message, const std::string& key) const;
};


class ExtractOnlyJsonBasedLatencyMonitoringMessageAdaptor: public JsonBasedLatencyMonitoringMessageAdaptor {
public:
    explicit ExtractOnlyJsonBasedLatencyMonitoringMessageAdaptor() {}

protected:
    std::string get_random_payload() override {
        throw ImproperUsageException();
    }
};


class JsonBasedLatencyMonitoringMessageGenerator: public JsonBasedLatencyMonitoringMessageAdaptor {
public:
    JsonBasedLatencyMonitoringMessageGenerator(int payload_size, int pre_indices_size);

    ~JsonBasedLatencyMonitoringMessageGenerator() = default;

protected:
    std::string get_random_payload();

private:
    static constexpr std::string_view PAYLOAD_CHARACTERS_ = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    static constexpr int MAX_CUR_IDX_MULTIPLIER_ = 10;

    int payload_size_;
    std::vector<int> pre_generated_indices_;
    int cur_idx_;

    void init_(int pre_indices_size);
};

class FastJsonBasedLatencyMonitoringMessageAdaptor: public ILatencyMonitoringMessageAdaptor {
protected:
    static constexpr std::string_view ID_KEY_ = "id";
    static constexpr std::string_view REQUESTED_AT_KEY = "requested_at";
    static constexpr char DIV_CHAR = '!';

    virtual std::string get_random_payload() = 0;
    virtual std::string get_payload_removed(const std::string& message) const;

public:
    std::string generate(std::string message_id) override;

    std::string generate(std::string messageId, double requested_at) override;

    virtual std::string generate(std::string messageId, double requested_at, std::map<std::string, std::string> oth_kvs);

    std::string extract_content(const std::string& message) const override;

    double extract_requested_at(const std::string& message) const override;

    virtual std::string extract_other_kvs(const std::string& message, const std::string& key) const;
};


class FastExtractOnlyJsonBasedLatencyMonitoringMessageAdaptor: public FastJsonBasedLatencyMonitoringMessageAdaptor {
public:
    explicit FastExtractOnlyJsonBasedLatencyMonitoringMessageAdaptor() {}

protected:
    std::string get_random_payload() override {
        throw ImproperUsageException();
    }
};


class FastJsonBasedLatencyMonitoringMessageGenerator: public FastJsonBasedLatencyMonitoringMessageAdaptor {
public:
    FastJsonBasedLatencyMonitoringMessageGenerator(int payload_size, int pre_indices_size);

    ~FastJsonBasedLatencyMonitoringMessageGenerator() = default;

protected:
    std::string get_random_payload();

private:
    static constexpr std::string_view PAYLOAD_CHARACTERS_ = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    static constexpr int MAX_CUR_IDX_MULTIPLIER_ = 10;

    int payload_size_;
    std::vector<int> pre_generated_indices_;
    int cur_idx_;

    void init_(int pre_indices_size);
};

}
}