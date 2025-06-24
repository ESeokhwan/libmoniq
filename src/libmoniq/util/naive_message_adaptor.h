#pragma once

#include "libmoniq/util/message_adaptor.h"

#include <string>

namespace monitor {
namespace util {

class NaiveMessageAdaptor : public IMessageAdaptor {
protected:
    static constexpr char div_char_ = '!';

    virtual std::string get_random_padding_(std::string message_id) = 0;

public:
    explicit NaiveMessageAdaptor() {}

    ~NaiveMessageAdaptor() = default;

    std::string generate(std::string message_id);

    std::string extract_message_id(const std::string& message);
};

}
}