#include "Bitmap.hpp"

#include <iostream>

namespace FS {
Bitmap::Bitmap(IOSystem& iosystem)
    : iosystem{iosystem}
    , first_use{true}
{
}

bool Bitmap::get(std::size_t index)
{
    if (first_use)
    {
        loadBitmap();
        first_use = false;
    }
    return data[index];
}

void Bitmap::set(size_t index, bool value)
{
    if (first_use)
    {
        loadBitmap();
        first_use = false;
    }
    data.set(index, value);
}

void Bitmap::loadBitmap()
{
    std::size_t curr_pos = 0;
    char* buffer = new char[Disk::BLOCK_SIZE];
    for (std::size_t curr_row = 0; curr_row <= Disk::BITMAP_SIZE_BYTES / Disk::BLOCK_SIZE;
         ++curr_row)
    {
        iosystem.read_block(curr_row, buffer);
        for (std::size_t j = 0; j < Disk::BLOCK_SIZE; ++j)
        {
            char byte = buffer[j];
            for (auto _ = 0; _ < sizeof(char) * 8; ++_)
            {
                if (curr_pos >= Disk::BITMAP_SIZE_BITS)
                {
                    return;
                }
                if (byte & 1)
                {
                    data.set(curr_pos);
                }
                byte >>= 1;
                ++curr_pos;
            }
        }
    }
    delete[] buffer;
}

void Bitmap::unloadBitmap()
{
    const size_t last_block_index = Disk::BITMAP_SIZE_BYTES / Disk::BLOCK_SIZE;
    size_t curr_pos = 0;
    char* buffer = new char[Disk::BLOCK_SIZE];
    for (size_t curr_row = 0; curr_row < last_block_index; ++curr_row)
    {
        for (auto index = 0; index < Disk::BLOCK_SIZE; ++index)
        {
            char a = 0;
            for (auto _ = 0; _ < sizeof(char) * 8; ++_)
            {
                if (data[curr_pos])
                {
                    a |= 0b10000000;
                }
                a >>= 1;
                ++curr_pos;
            }
            buffer[index] = a;
        }
        iosystem.write_block(curr_row, buffer);
    }
    if (curr_pos >= Disk::BITMAP_SIZE_BITS)
    {
        return;
    }
    iosystem.read_block(last_block_index, buffer);
    for (auto index = 0; index < Disk::BLOCK_SIZE; ++index)
    {
        char a = 0;
        for (auto _ = 0; _ < 7; ++_)
        {
            if (data[curr_pos])
            {
                a |= 0b10000000;
            }
            a >>= 1;
            a &= 0b01111111;
            ++curr_pos;
            if (curr_pos >= Disk::BITMAP_SIZE_BITS)
            {
                buffer[index] = a;
                iosystem.write_block(last_block_index, buffer);
                delete[] buffer;
                return;
            }
        }
        if (data[curr_pos])
        {
            a |= 0b10000000;
        }
        ++curr_pos;
        if (curr_pos >= Disk::BITMAP_SIZE_BITS)
            {
                buffer[index] = a;
                iosystem.write_block(last_block_index, buffer);
                delete[] buffer;
                return;
            }
        buffer[index] = a;
    }
    iosystem.write_block(last_block_index, buffer);
    delete[] buffer;
}

void Bitmap::reset()
{
    if (first_use)
    {
        loadBitmap();
    }
    data.set(0, true); // always blocked for directory file
    for (size_t index = 1; index < data.size(); ++index)
    {
        data.set(index, false);
    }
    unloadBitmap();
}

Bitmap::~Bitmap()
{
    unloadBitmap();
}

} // namespace FS