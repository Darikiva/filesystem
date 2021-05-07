#include <iostream>
#include <sstream>
#include <fstream>

#include "Controller.hpp"


int main()
{
    FS::Controller controller("saving.bin");
    controller.reset();
    std::ifstream in("test_fs_overflow.txt");
    std::ofstream out("test_fs_overflow_output.txt");
    controller.work(in, out);
    in.close();
    out.close();
    return 0;
}