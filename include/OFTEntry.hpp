#pragma once

#include <vector>

#include "Disk.hpp"
#include "Entities.hpp"
#include "IOSystem.hpp"

namespace FS {

class OFTEntry
{
public:
    OFTEntry(Entity::FileDescriptor file_descriptor)
        : file_descriptor(file_descriptor),iosystem(iosystem)
    {
    }
    void setBuffer(char* buffer);
    void readBuffer();

private:
    char buffer[Disk::BLOCK_SIZE];
    size_t cur_pos = 0;
    Entity::FileDescriptor file_descriptor;
    size_t cur_block;
    IOSystem& iosystem;
    bool isBufferChanged = false;
    void updateBuffer();
};

} // namespace FS