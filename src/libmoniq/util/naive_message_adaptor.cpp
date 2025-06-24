#include "libmoniq/util/naive_message_adaptor.h"

namespace monitor {
namespace util {

std::string NaiveMessageAdaptor::generate(std::string message_id) {
    std::string padding = get_random_padding_(message_id);
    return message_id + div_char_ + padding;
}

std::string NaiveMessageAdaptor::extract_message_id(const std::string& message) {
    size_t message_id_size = message.rfind(div_char_);
    if (message_id_size == std::string::npos) {
        message_id_size = message.length();
    }
    return message.substr(0, message_id_size);
}

}
}