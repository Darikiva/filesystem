#include "Descriptors.hpp"
#include "Entities.hpp"
#include <iostream>

namespace FS {

Descriptors::Descriptors(IOSystem& iosystem)
    : iosystem{iosystem}
    , first_use{true}
{
}

const Entity::FileDescriptor& Descriptors::get(std::size_t index)
{
    if (first_use)
    {
        loadDescriptors();
        first_use = false;
    }
    return data[index];
}

void Descriptors::set(size_t index, const Entity::FileDescriptor& value)
{
    if (first_use)
    {
        loadDescriptors();
        first_use = false;
    }
    data[index] = value;
    unloadDescriptor(index);
}

void Descriptors::loadDescriptors() // load all descriptors to dara vector
{
    std::size_t pos = Disk::BITMAP_SIZE_BYTES % Disk::BLOCK_SIZE;
    const std::size_t block_start_index = Disk::BITMAP_SIZE_BYTES / Disk::BLOCK_SIZE;
    char* buffer = new char[Disk::BLOCK_SIZE];
    for (std::size_t index = block_start_index; index < Disk::K; ++index)
    {
        iosystem.read_block(index, buffer);
        while (pos + descriptor_size <= Disk::BLOCK_SIZE)
        {
            auto desc_p = reinterpret_cast<Entity::FileDescriptor*>(&buffer[pos]);
            data.push_back(*desc_p);
            pos += descriptor_size;
        }
        pos = 0;
    }
    delete[] buffer;
}

void Descriptors::unloadDescriptors() // write all descriptors to disk
{
    char* buffer = new char[Disk::BLOCK_SIZE];
    size_t curr_row = Disk::BITMAP_SIZE_BYTES / Disk::BLOCK_SIZE;
    size_t curr_byte = Disk::BITMAP_SIZE_BYTES % Disk::BLOCK_SIZE;
    size_t curr_data_pos = 0;
    for (; curr_row < Disk::K; ++curr_row)
    {
        iosystem.read_block(curr_row, buffer);
        while (curr_byte + descriptor_size <= Disk::BLOCK_SIZE)
        {
            if (curr_data_pos >= data.size())
            {
                iosystem.write_block(curr_row, buffer);
                delete[] buffer;
                return;
            }
            auto ch_p = reinterpret_cast<char*>(&data[curr_data_pos]);
            for (size_t index = 0; index < descriptor_size; ++index)
            {
                buffer[curr_byte] = ch_p[index];
                ++curr_byte;
            }
            ++curr_data_pos;
        }
        curr_byte = 0;
        iosystem.write_block(curr_row, buffer);
    }
    delete[] buffer;
}

void Descriptors::unloadDescriptor(size_t index) // write descriptor to disk
{
    const size_t row =
        index <= (Disk::BLOCK_SIZE - Disk::BITMAP_SIZE_BYTES) / descriptor_size
            ? 0
            : (index - (Disk::BLOCK_SIZE - Disk::BITMAP_SIZE_BYTES) / descriptor_size) /
                      (Disk::BLOCK_SIZE / descriptor_size) +
                  1;
    size_t byte =
        index <= (Disk::BLOCK_SIZE - Disk::BITMAP_SIZE_BYTES) / descriptor_size
            ? Disk::BITMAP_SIZE_BYTES + index * descriptor_size
            : (index - (Disk::BLOCK_SIZE - Disk::BITMAP_SIZE_BYTES) / descriptor_size) %
                  (Disk::BLOCK_SIZE / descriptor_size) * descriptor_size;
    char* buffer = new char[Disk::BLOCK_SIZE];
    iosystem.read_block(row, buffer);
    auto ch_p = reinterpret_cast<char*>(&data[index]);
    for (size_t index = 0; index < descriptor_size; ++index)
    {
        buffer[byte] = ch_p[index];
        ++byte;
    }
    iosystem.write_block(row, buffer);
    delete[] buffer;
}

void Descriptors::reset()
{
    if (first_use)
    {
        loadDescriptors();
    }
    Entity::FileDescriptor directory_desc = {0, {0, -1, -1}};
    data[0] = directory_desc;
    for (size_t index = 1; index < data.size(); ++index)
    {
        data[index] = {0, {-1, -1, -1}};
    }
    unloadDescriptors();
}

Descriptors::~Descriptors()
{
    unloadDescriptors();
}
} // namespace FS