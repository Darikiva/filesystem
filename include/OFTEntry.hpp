#pragma once

#include <vector>

#include "Disk.hpp"
#include "Entities.hpp"
#include "IOSystem.hpp"

namespace FS {

class OFTEntry
{
public:
    OFTEntry(Entity::FileDescriptor file_descriptor, IOSystem* iosystem)
        : file_descriptor(file_descriptor),iosystem(iosystem)
    {
        cur_block = file_descriptor.indexes[0];
        changed_block = cur_block;
    }
    void setBuffer(const char* new_buffer);
    const char* readBuffer();
    void setPosition(size_t new_pos);
    void onClose();
    bool isEmpty();

private:
    char buffer[Disk::BLOCK_SIZE];
    size_t cur_pos = 0;
    Entity::FileDescriptor file_descriptor;
    size_t changed_block;
    size_t cur_block;
    IOSystem* iosystem;
    bool is_buffer_changed = false;
    void updateBuffer();
};

} // namespace FS