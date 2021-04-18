#include "IOSystem.hpp"

#include <bitset>
#include <cwchar>
#include <fstream>
#include <iostream>

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

void IOSystem::write_block(std::size_t i, char* p)
{
    char* destination = disk.ldisk[i];
    for (std::size_t j = 0; j < Disk::BLOCK_SIZE; ++j)
    {
        destination[j] = p[j];
    }
}


} // namespace FS
