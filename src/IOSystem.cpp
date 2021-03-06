#include "IOSystem.hpp"

#include <iostream>
#include <cwchar>

namespace FS {

IOSystem::IOSystem(Disk& disk)
    : disk{disk}
{
}

void IOSystem::read_block(std::size_t i, char* p)
{
    const char* data = disk.ldisk[i];
    for (std::size_t j = 0; j < Disk::BLOCK_SIZE; ++j)
    {
        p[j] = data[j];
    }
}

void IOSystem::write_block(size_t i, char* p)
{
    char* destination = disk.ldisk[i];
    for (size_t j = 0; j < Disk::BLOCK_SIZE; ++j)
    {
        destination[j] = p[j];
    }
}

} // namespace FS
