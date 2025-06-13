#pragma once

#include <string>

namespace monitor {
namespace util {

class IMessageAdaptor {
public:
    virtual std::string generate(std::string messageId) = 0;

    virtual std::string extract_message_id(const std::string& message) = 0;
};

}
}