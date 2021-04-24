#include <iostream>
#include <sstream>
#include <fstream>

#include "Controller.hpp"


int main()
{
    FS::Controller controller("saving.bin");
    controller.reset();
    controller.work(std::cin, std::cout);
    
    return 0;
}