#include "Directories.hpp"
#include "Entities.hpp"
#include <iostream>

namespace FS {

Directories::Directories(IOSystem& iosystem)
    : iosystem{iosystem}
    , first_use{true}
{
}

Entity::DirectoryEntry& Directories::get(std::size_t index)
{
    if (first_use)
    {
        loadDirectories();
        first_use = false;
    }
    return data[index];
}

void Directories::set(size_t index, const Entity::DirectoryEntry& value)
{
    if (first_use)
    {
        loadDirectories();
        first_use = false;
    }
    data[index] = value;
    unloadDirectories();
}

void Directories::loadDirectories()
{
    std::size_t pos = 0;
    const std::size_t block_start_index = Disk::K;
    char* buffer = new char[Disk::BLOCK_SIZE];

    iosystem.read_block(block_start_index, buffer);
    while (pos + directory_size <= Disk::BLOCK_SIZE)
    {
        auto dir_p = reinterpret_cast<Entity::DirectoryEntry*>(&buffer[pos]);
        data.push_back(*dir_p);
        pos += directory_size;
    }

    delete[] buffer;
}

void Directories::unloadDirectories()
{
    char* buffer = new char[Disk::BLOCK_SIZE];
    size_t curr_row = Disk::K;
    size_t curr_byte = 0;
    size_t curr_data_pos = 0;

    iosystem.read_block(curr_row, buffer);
    while (curr_byte + directory_size <= Disk::BLOCK_SIZE)
    {
        if (curr_data_pos >= data.size())
        {
            iosystem.write_block(curr_row, buffer);
            delete[] buffer;
            return;
        }
        auto ch_p = reinterpret_cast<char*>(&data[curr_data_pos]);
        for (size_t index = 0; index < directory_size; ++index)
        {
            buffer[curr_byte] = ch_p[index];
            ++curr_byte;
        }
        ++curr_data_pos;
    }
    iosystem.write_block(curr_row, buffer);

    delete[] buffer;
}

void Directories::unloadDirectory(size_t index)
{
    if (index <= Disk::BLOCK_SIZE / directory_size) {
        return;
    }
    const size_t row = Disk::K;
    size_t byte = index * directory_size;
    char* buffer = new char[Disk::BLOCK_SIZE];
    iosystem.read_block(row, buffer);
    auto ch_p = reinterpret_cast<char*>(&data[index]);
    for (size_t i = 0; i < directory_size; ++i)
    {
        buffer[byte] = ch_p[i];
        ++byte;
    }
    iosystem.write_block(row, buffer);
    delete[] buffer;
}

void Directories::reset()
{
    if (first_use)
    {
        loadDirectories();
    }
    for (size_t index = 0; index < size(); ++index)
    {
        data[index] = {-1, {}};
    }
    unloadDirectories();
}

Directories::~Directories()
{
    unloadDirectories();
}
} // namespace FS
