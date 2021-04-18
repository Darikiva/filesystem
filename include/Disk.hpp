#pragma once

#include <bitset>
#include <cstdint>
#include <string_view>
#include <vector>

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
    static constexpr size_t BITMAP_SIZE_BITS = BLOCKS_AMOUNT - K;
    // clang-format off
    static constexpr size_t BITMAP_SIZE_BYTES = BITMAP_SIZE_BITS / 8 == 0
                                                ? BITMAP_SIZE_BITS / 8
                                                : BITMAP_SIZE_BITS / 8 + 1;
    // clang-format on

private:
    char (*ldisk)[BLOCK_SIZE];
    std::string save_name;
    void save();
    friend class IOSystem;
};
} // namespace FS
