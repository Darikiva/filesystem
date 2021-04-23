#include <cstdint>
#include <iostream>

#include "Controller.hpp"
#include "Descriptors.hpp"

int main()
{
    FS::Controller controller("saving.txt");
    controller.reset();
    char* buffer = new char[FS::Disk::BLOCK_SIZE * 3];
    controller.filesystem->create("hell");
    std::pair<FS::Status, size_t> result = controller.filesystem->open("hell");
    std::cout << FS::toString(result.first);
    return 0;
}