#pragma once

#include "libmoniq/util/naive_message_adaptor.h"

#include <stdexcept>
#include <string>

namespace monitor {
namespace util {

class ExtractOnlyNaiveMessageAdaptor : public NaiveMessageAdaptor {
public:
    explicit ExtractOnlyNaiveMessageAdaptor() {}

protected:
    std::string get_random_padding_(std::string message_id) override {
        throw std::runtime_error("This class is not for generating message. It is just for extracting message.");
    }
};

}
}
