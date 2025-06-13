#pragma once

#include "libmoniq/util/naive_message_adaptor.h"

#include <stdexcept>
#include <string>

namespace monitor {
namespace util {

class ExtractOnlyNaiveMessageAdaptor : public NaiveMessageAdaptor {
public:
    explicit ExtractOnlyNaiveMessageAdaptor(int message_size);

protected:
    std::string get_random_padding(int size) {
        throw std::runtime_error("This class is not for generating message. It is just for extracting message.");
    }
};

}
}
