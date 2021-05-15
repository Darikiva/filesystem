#include <iostream>
#include <sstream>
#include <fstream>

#include "Controller.hpp"


int main()
{
    FS::Controller controller("saving.bin");
    controller.reset();
//    std::ifstream in("test/test_end_of_file.txt");
//    std::ofstream out("test/test_end_of_file_output.txt");
    std::ifstream in("../test/test-sample.txt");
    std::ofstream out("../test/output-sample.txt");
    controller.work(in, out);
    in.close();
    out.close();
    return 0;
}