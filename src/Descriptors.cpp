#include "Descriptors.hpp"
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
}

void Descriptors::loadDescriptors()
{
    std::size_t pos = Disk::bitmap_size_bytes % Disk::BLOCK_SIZE;
    const std::size_t block_start_index = Disk::bitmap_size_bytes / Disk::BLOCK_SIZE;
    static constexpr std::size_t descriptor_size_bytes = sizeof(Entity::FileDescriptor);
    char* buffer = new char[Disk::BLOCK_SIZE];
    for (std::size_t index = block_start_index; index < Disk::K; ++index)
    {
        iosystem.read_block(index, buffer);
        while (pos + descriptor_size_bytes <= Disk::BLOCK_SIZE)
        {
            auto desc_p = reinterpret_cast<Entity::FileDescriptor*>(&buffer[pos]);
            data.push_back(*desc_p);
            pos += descriptor_size_bytes;
        }
        pos = 0;
    }
    delete[] buffer;
}


void Descriptors::unloadDescriptors()
{
    char* buffer = new char[Disk::BLOCK_SIZE];
    size_t curr_row = Disk::bitmap_size_bytes / Disk::BLOCK_SIZE;
    size_t curr_byte = Disk::bitmap_size_bytes % Disk::BLOCK_SIZE;
    size_t curr_data_pos = 0;
    static constexpr size_t descriptor_size_bytes = sizeof(Entity::FileDescriptor);
    iosystem.read_block(curr_row, buffer);
    for (; curr_row < Disk::K; ++curr_row)
    {
        while (curr_byte + descriptor_size_bytes <= Disk::BLOCK_SIZE)
        {
            if (curr_data_pos >= data.size()) 
            {
                iosystem.write_block(curr_row, buffer);
                return;
            }
            auto ch_p = reinterpret_cast<char*>(&data[curr_data_pos]);
            for (size_t index = 0; index < descriptor_size_bytes; ++index)
            {
                buffer[curr_byte] = ch_p[index];
                ++curr_byte;
            }
            ++curr_data_pos;
        }
        curr_byte = 0;
    }
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
        data[index] = {0, {-1, 1, -1}};
    }
    unloadDescriptors();
}

Descriptors::~Descriptors()
{
    unloadDescriptors();
}
} // namespace FS