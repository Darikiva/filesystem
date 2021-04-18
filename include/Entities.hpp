#pragma once

#include <cstdint>
#include "Disk.hpp"

#include "Bitmap.hpp"
#include "Disk.hpp"

namespace FS {
namespace Entity {

struct DirectoryEntry
{
    std::uint8_t descriptor_index;
    char file_name[4];
};

struct FileDescriptor
{
    std::uint8_t file_length;
    std::int8_t indexes[3];
};

struct OFTEntry
{
    char buffer[64]; // BLOCK_SIZE
    size_t cur_pos; // initially zero
    size_t file_descriptor_index;
    bool isBufferChanged;
};

struct OFT {
    std::vector<OFTEntry> entries;
};


} // namespace Entity
} // namespace FS