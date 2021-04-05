#include <cstdint>
#include <iostream>

#include "Controller.hpp"
#include "Descriptors.hpp"

int main()
{
    FS::Controller controller("saving.bin");
    controller.reset();
    return 0;
}