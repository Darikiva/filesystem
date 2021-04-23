#include "OFTEntry.hpp"

namespace FS {

void OFTEntry::setBuffer(const char* new_buffer)
{
    if (is_buffer_changed)
    {
        iosystem->write_block(changed_block, buffer);
    }

    // TODO CARRYOVER
    for (int i = cur_pos; i < sizeof(new_buffer) + cur_pos && i < Disk::BLOCK_SIZE; i++)
    {
        buffer[i] = new_buffer[i];
    }
    is_buffer_changed = true;
    changed_block = cur_block;
}

const char* OFTEntry::readBuffer()
{
    if (is_buffer_changed)
    {
        iosystem->write_block(changed_block, buffer);
        updateBuffer();
    }
    return buffer;
}

void OFTEntry::updateBuffer()
{
    iosystem->read_block(cur_block, buffer);
    is_buffer_changed = false;
}

void OFTEntry::setPosition(size_t new_pos)
{
    cur_pos = new_pos % (Disk::BLOCK_SIZE * cur_block);
    if (file_descriptor.indexes[new_pos % Disk::BLOCK_SIZE] != cur_block)
    {
        cur_block = file_descriptor.indexes[new_pos % Disk::BLOCK_SIZE];
    }
}

void OFTEntry::onClose()
{
    if (is_buffer_changed)
    {
        iosystem->write_block(changed_block, buffer);
    }
    else
    {
        iosystem->write_block(cur_block, buffer);
    }
}

bool OFTEntry::isEmpty()
{
    return file_descriptor.indexes[0] == -1;
}
}; // namespace FS