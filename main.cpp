#include <iostream>
#include <sstream>
#include <fstream>

#include "Controller.hpp"


int main()
{
    std::string IN[] = {
        "../test/test_fs_overflow.txt",
        "../test/test_end_of_file.txt",
        "../test/test-sample.txt",
        "../test/custom-test-destroy.txt"
    };

    std::string OUT[] = {
        "../test/test_fs_overflow_output.txt",
        "../test/test_end_of_file_output.txt",
        "../test/output-sample.txt",
        "../test/output-destroy.txt"
    };

    FS::Controller controller("saving.bin");
    controller.reset();
    std::ifstream in(IN[1]);
    std::ofstream out(OUT[1]);
    controller.work(in, out);
    in.close();
    out.close();
    return 0;
}