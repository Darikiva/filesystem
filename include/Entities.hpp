#pragma once

#include <cstdint>

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

} // namespace Entity
} // namespace FS