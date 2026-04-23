#include "libmoniq/adaptor/naive_message_adaptor.h"

#include <random>

namespace moniq {
namespace adaptor {

std::string NaiveMessageAdaptor::generate(std::string content) {
    std::string padding = get_random_padding_(content);
    return content + div_char_ + padding;
}

std::string NaiveMessageAdaptor::extract_content(const std::string& message) const {
    size_t content_size = message.rfind(div_char_);
    if (content_size == std::string::npos) {
        content_size = message.length();
    }
    return message.substr(0, content_size);
}

NaiveMessageGenerator::NaiveMessageGenerator(int message_size, int pre_indices_size)
    : NaiveMessageAdaptor(), message_size_(message_size), cur_idx_(0) {
    init_(pre_indices_size);
}

void NaiveMessageGenerator::init_(int pre_indices_size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, static_cast<int>(PADDING_CHARACTERS_.size()) - 1);

    int total_pool_size = std::max(pre_indices_size, message_size_);
    pre_generated_payload_.reserve(total_pool_size);
    for (int i = 0; i < total_pool_size; ++i) {
        pre_generated_payload_ += PADDING_CHARACTERS_[dist(gen)];
    }
}

std::string NaiveMessageGenerator::get_random_padding_(std::string content) {
    int padding_size = message_size_ - static_cast<int>(content.length()) - 1;
    int pool_size = static_cast<int>(pre_generated_payload_.size()) - padding_size;
    if (cur_idx_ >= pool_size) {
        cur_idx_ = 0;
    }
    std::string result = pre_generated_payload_.substr(cur_idx_, padding_size);
    cur_idx_++;
    return result;
}

}
}