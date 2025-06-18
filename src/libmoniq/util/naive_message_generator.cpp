#include "libmoniq/util/naive_message_generator.h"

#include <random>
#include <sstream>

namespace monitor {
namespace util {

const int MAX_CUR_IDX_MULTIPLIER = 10;

const std::string NaiveMessageGenerator::padding_characters =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

NaiveMessageGenerator::NaiveMessageGenerator(int message_size, int pre_indices_size)
    : NaiveMessageAdaptor(message_size), cur_idx_(0) {
    init_(pre_indices_size);
}

void NaiveMessageGenerator::init_(int pre_indices_size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, static_cast<int>(padding_characters.size()) - 1);

    pre_generated_indices_.resize(pre_indices_size);
    cur_idx_ = 0;
    for (int& idx : pre_generated_indices_) {
        idx = dist(gen);
    }
}

std::string NaiveMessageGenerator::get_random_padding_(int size) {
    std::ostringstream padded_string;
    for (int i = 0; i < size; i++) {
        int pre_generated_size = static_cast<int>(pre_generated_indices_.size());
        if (cur_idx_ >= pre_generated_size * MAX_CUR_IDX_MULTIPLIER) {
            cur_idx_ = 0;
        }
        char random_char = padding_characters[pre_generated_indices_[cur_idx_ % pre_generated_size]];
        padded_string << random_char;
        cur_idx_ += 1;
    }
    return padded_string.str();
}

}
}