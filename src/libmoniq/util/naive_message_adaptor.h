#pragma once

#include "libmoniq/util/message_adaptor.h"

#include <string>

namespace monitor {
namespace util {

class NaiveMessageAdaptor : public IMessageAdaptor {
protected:
    static constexpr char div_char_ = '!';
    const int message_size_;

    virtual std::string get_random_padding_(int paddingSize) = 0;

public:
    explicit NaiveMessageAdaptor(int message_size)
        : message_size_(message_size) {}

    ~NaiveMessageAdaptor() = default;

    std::string generate(std::string messageId);

    std::string extract_message_id(const std::string& message);
};

}
}