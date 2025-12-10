#pragma once

#include "libmoniq/exception/common_exceptions.h"
#include "libmoniq/util/message_adaptor.h"

#include <string>
#include <vector>

namespace moniq {
namespace util {

class NaiveMessageAdaptor : public IMessageAdaptor {
protected:
    static constexpr char div_char_ = '!';

    virtual std::string get_random_padding_(std::string content) = 0;

public:
    explicit NaiveMessageAdaptor() {}

    ~NaiveMessageAdaptor() = default;

    std::string generate(std::string content) override;

    std::string extract_content(const std::string& message) const override;
};


class ExtractOnlyNaiveMessageAdaptor : public NaiveMessageAdaptor {
public:
    explicit ExtractOnlyNaiveMessageAdaptor() {}

protected:
    std::string get_random_padding_([[maybe_unused]] std::string content) override {
        throw ImproperUsageException();
    }
};


class NaiveMessageGenerator : public NaiveMessageAdaptor {
public:
    NaiveMessageGenerator(int message_size, int pre_indices_size);

    ~NaiveMessageGenerator() = default;

protected:
    std::string get_random_padding_(std::string content) override;

private:
    static constexpr std::string_view PADDING_CHARACTERS_ = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    static constexpr int MAX_CUR_IDX_MULTIPLIER_ = 10;

    int message_size_;
    std::vector<int> pre_generated_indices_;
    int cur_idx_;

    void init_(int pre_indices_size);
};

}
}