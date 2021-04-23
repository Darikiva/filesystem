#include <iostream>

#include "Controller.hpp"


int main()
{
    FS::Controller controller("saving.bin");
    controller.reset();
    char* buffer = new char[FS::Disk::BLOCK_SIZE * 3];
    char* buffer_res = new char[FS::Disk::BLOCK_SIZE * 3];
    controller.filesystem->create("hell");
    int oft = controller.filesystem->open("hell").second;
    for (int i = 0; i < 5; i++)
    {
        buffer[i] = 'a' + i;
    }
    auto result = controller.filesystem->write(oft, buffer, 3);
    controller.filesystem->lseek(oft, 0);
    controller.filesystem->read(oft, buffer_res, 2);
    controller.filesystem->lseek(oft, FS::Disk::BLOCK_SIZE + 10);
    controller.filesystem->lseek(oft, FS::Disk::BLOCK_SIZE * 3 - 2);
    result = controller.filesystem->write(oft, buffer, 4);
    std::cout << FS::toString(result.first) + ' ' + result.second;
    controller.filesystem->lseek(oft, FS::Disk::BLOCK_SIZE * 3 - 2);
    result = controller.filesystem->read(oft, buffer_res, 4);
    std::cout << FS::toString(result.first) + ' ' + result.second;
    controller.filesystem->lseek(oft, 0);
    result = controller.filesystem->read(oft, buffer_res, 3);
    std::cout << FS::toString(result.first) + ' ' + result.second;
    return 0;
}