#include "libmoniq/adaptor/naive_message_adaptor.h"

#include <random>
#include <sstream>

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

    pre_generated_indices_.resize(pre_indices_size);
    cur_idx_ = 0;
    for (int& idx : pre_generated_indices_) {
        idx = dist(gen);
    }
}

std::string NaiveMessageGenerator::get_random_padding_(std::string content) {
    int padding_size = message_size_ - static_cast<int>(content.length()) - 1;
    std::ostringstream padded_string;
    for (int i = 0; i < padding_size; i++) {
        int pre_generated_size = static_cast<int>(pre_generated_indices_.size());
        if (cur_idx_ >= pre_generated_size * MAX_CUR_IDX_MULTIPLIER_) {
            cur_idx_ = 0;
        }
        char random_char = PADDING_CHARACTERS_[pre_generated_indices_[cur_idx_ % pre_generated_size]];
        padded_string << random_char;
        cur_idx_ += 1;
    }
    return padded_string.str();
}

}
}