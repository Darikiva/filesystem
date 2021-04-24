#include "OFTEntry.hpp"

#include <algorithm>

namespace FS {

size_t OFTEntry::getCurrBlockIndex()
{
    size_t curr_block_index = 0;
    for (size_t i = 1; i < sizeof(file_descriptor.indexes); i++)
    {
        if (file_descriptor.indexes[i] == cur_block)
        {
            curr_block_index = i;
        }
    }
    return curr_block_index;
}

size_t OFTEntry::getNextBlock()
{
    size_t curr_block_index = getCurrBlockIndex();
    if (curr_block_index == sizeof(file_descriptor.indexes) - 1)
    {
        return -1;
    }

    return file_descriptor.indexes[curr_block_index + 1];
}

bool OFTEntry::moveToNextBlock(bool load_prev_to_disk)
{
    size_t next_block = getNextBlock();
    if (next_block == -1)
    {
        return false;
    }
    if (load_prev_to_disk)
    {
        iosystem->write_block(cur_block, buffer);
    }
    cur_block = next_block;
    cur_pos = 0;
    updateBuffer();
    return true;
}

size_t OFTEntry::getLengthByCurPos()
{
    size_t curr_block_index = getCurrBlockIndex();

    return curr_block_index * Disk::BLOCK_SIZE + cur_pos;
}

int OFTEntry::writeToBuffer(const char* mem_area, int count)
{
    // if cur_pos on end of file
    if (cur_pos == Disk::BLOCK_SIZE)
    {
        return 0;
    }

    if (is_buffer_empty)
    {
        updateBuffer();
        is_buffer_empty = false;
    }
    // read actual block to buffer
    if (is_buffer_changed)
    {
        iosystem->write_block(changed_block, buffer);
        updateBuffer();
    }

    int number_of_written = 0;
    for (size_t i = 0; i < count; i++)
    {
        buffer[cur_pos] = mem_area[i];

        ++cur_pos;
        // reached end of block
        if (cur_pos == Disk::BLOCK_SIZE)
        {
            bool block_updated = moveToNextBlock(true);
            // reached end of file
            if (!block_updated)
            {
                number_of_written = i + 1;
                break;
            }
        }
    }
    if (number_of_written == 0)
    {
        number_of_written = count;
    }
    changed_block = cur_block;
    file_descriptor.file_length = getLengthByCurPos();
    return number_of_written;
}

std::pair<std::string, int> OFTEntry::readFromBuffer(int count)
{
    // if cur_pos on end of file
    if (cur_pos == Disk::BLOCK_SIZE)
    {
        return {std::string(), 0};
    }

    if (is_buffer_empty)
    {
        updateBuffer();
        is_buffer_empty = false;
    }

    if (is_buffer_changed)
    {
        iosystem->write_block(changed_block, buffer);
        updateBuffer();
    }

    std::string result_arr(count, ' ');
    int number_of_read = 0;
    for (size_t i = 0; i < count; i++)
    {
        if (cur_pos >= file_descriptor.file_length)
        {
            break;
        }
        result_arr[i] = buffer[cur_pos];
        ++number_of_read;

        ++cur_pos;
        // reached end of block
        if (cur_pos == Disk::BLOCK_SIZE)
        {
            bool block_updated = moveToNextBlock(false);
            // reached end of file
            if (!block_updated)
            {
                break;
            }
        }
    }

    return {result_arr, number_of_read};
}

void OFTEntry::updateBuffer()
{
    iosystem->read_block(cur_block, buffer);
    is_buffer_changed = false;
}

size_t OFTEntry::setPosition(size_t new_pos)
{
    new_pos = new_pos > file_descriptor.file_length ? file_descriptor.file_length : new_pos;
    cur_pos = new_pos % Disk::BLOCK_SIZE;
    if (file_descriptor.indexes[new_pos / Disk::BLOCK_SIZE] != cur_block)
    {
        is_buffer_changed = true;
        cur_block = file_descriptor.indexes[new_pos / Disk::BLOCK_SIZE];
    }
    return new_pos;
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

bool OFTEntry::isEmpty() const
{
    return descriptor_index == -1;
}

size_t OFTEntry::getDescriptorIndex() const
{
    return descriptor_index;
}

Entity::FileDescriptor OFTEntry::getDescriptor()
{
    return file_descriptor;
}
}; // namespace FS