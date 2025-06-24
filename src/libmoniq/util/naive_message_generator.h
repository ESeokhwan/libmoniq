#pragma once

#include "libmoniq/util/naive_message_adaptor.h"

#include <vector>
#include <string>

namespace monitor {
namespace util {

class NaiveMessageGenerator : public NaiveMessageAdaptor {
public:
    NaiveMessageGenerator(int message_size, int pre_indices_size);

    ~NaiveMessageGenerator() = default;

protected:
    std::string get_random_padding_(std::string message_id) override;

private:
    static const std::string padding_characters;

    int message_size_;
    std::vector<int> pre_generated_indices_;
    int cur_idx_;

    void init_(int pre_indices_size);
};

}
}
