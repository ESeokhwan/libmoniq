#pragma once

#include <string>
#include <stdexcept>

namespace moniq {

class MoniqException: public std::exception {
private:
    std::string message_;

public:
    MoniqException(std::string message): message_(message) {}

    const char* what() const noexcept override {
        return message_.c_str();
    }
};

class MoniqRuntimeException: public std::runtime_error {
private:
    std::string message_;

public:
    MoniqRuntimeException(std::string message): std::runtime_error(message.c_str()), message_(message) {}
};

class NotProcessedException: public MoniqRuntimeException {
public:
    NotProcessedException(): MoniqRuntimeException("Not processed yet.") {}
};

class ImproperUsageException: public MoniqRuntimeException {
public:
    ImproperUsageException(): MoniqRuntimeException("Improper usage.") {}
};

}