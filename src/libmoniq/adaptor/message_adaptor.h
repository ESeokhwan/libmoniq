#pragma once

#include <string>

namespace moniq {
namespace adaptor {

class IMessageAdaptor {
public:
    virtual std::string generate(std::string content) = 0;

    virtual std::string extract_content(const std::string& message) const = 0;
};

}
}