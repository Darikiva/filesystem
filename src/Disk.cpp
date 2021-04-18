#include "Disk.hpp"

#include <fstream>
#include <iostream>

namespace FS {

Disk::Disk(std::string save_name)
    : save_name{save_name}
{
    ldisk = new char[BLOCKS_AMOUNT][BLOCK_SIZE];
    std::ifstream in(save_name, std::ios::binary | std::ios::in);
    for (std::size_t index = 0; index < BLOCKS_AMOUNT; ++index)
    {
        in.read(ldisk[index], BLOCK_SIZE);
    }
    in.close();
}

Disk::~Disk()
{
    save();
}

void Disk::save()
{
    std::ofstream out(save_name, std::ios::binary | std::ios::out);
    for (std::size_t index = 0; index < BLOCKS_AMOUNT; ++index)
    {
        out.write(ldisk[index], BLOCK_SIZE);
    }
    out.close();
}

// void Disk::reset()
// {
//     std::size_t curr_byte = 0;
//     std::size_t curr_row = 0;
//     for (; curr_byte < BITMAP_SIZE_BYTES; ++curr_byte)
//     {
//         curr_row = (curr_byte) / BLOCK_SIZE;
//         ldisk[curr_row][curr_byte % BLOCK_SIZE] = 'e';
//     }
//     curr_byte %= BLOCK_SIZE;
//     static Entity::FileDescriptor empty_desc;
//     char* data = reinterpret_cast<char*>(&empty_desc);
//     static constexpr size_t descriptor_size_bytes = sizeof (Entity::FileDescriptor);
//     while (curr_row < K)
//     {
//         while(curr_byte + descriptor_size_bytes < BLOCK_SIZE)
//         {
//             for(size_t j = 0; j < descriptor_size_bytes; ++j)
//             {
//                 ldisk[curr_row][curr_byte] = data[j];
//                 ++curr_byte;
//             }
//         }
//         curr_byte = 0;
//         ++curr_row;
//     }
// }
} // namespace FS