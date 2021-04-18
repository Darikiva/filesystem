#pragma once

#include <cstdint>

#include "Bitmap.hpp"
#include "Disk.hpp"

namespace FS {
namespace Entity {

struct DirectoryEntry
{
    std::int8_t descriptor_index;
    char file_name[4];
};

// struct Directory
//{
//    std::vector<DirectoryEntry> directory_entries;
//};

struct FileDescriptor
{
    std::uint8_t file_length;
    std::int8_t indexes[3];
};

struct OFTEntry
{
    char buffer[Disk::BLOCK_SIZE]; // BLOCK_SIZE
    size_t cur_pos;                // initially zero
    size_t file_descriptor_index;
    bool isBufferChanged;
};

struct OFT
{
    std::vector<OFTEntry> entries;
};

} // namespace Entity
} // namespace FS