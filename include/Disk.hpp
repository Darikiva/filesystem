#pragma once

#include <bitset>
#include <cstdint>
#include <string_view>
#include <vector>

#include "Entities.hpp"

namespace FS {

class Disk
{
public:
    Disk(std::string save_name);
    ~Disk();

    // void reset();

public:
    // TODO: rename constants
    static constexpr size_t BLOCK_SIZE = 64;
    static constexpr size_t K = 2;
    static constexpr size_t BLOCKS_AMOUNT = 32;
    static constexpr size_t bitmap_size_bits = BLOCKS_AMOUNT - K;
    // clang-format off
    static constexpr size_t bitmap_size_bytes = bitmap_size_bits / 8 == 0
                                                ? bitmap_size_bits / 8 
                                                : bitmap_size_bits / 8 + 1;
    // clang-format on
private:
    char (*ldisk)[BLOCK_SIZE];
    std::string save_name;
    void save();
    friend class IOSystem;
};
} // namespace FS
