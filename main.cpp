#include <iostream>
#include <sstream>
#include <fstream>

#include "Controller.hpp"


int main()
{
    std::string IN[] = {
        "../test/test_fs_overflow.txt",         //0
        "../test/test_end_of_file.txt",         //1
        "../test/test-sample.txt",              //2
        "../test/custom-test-destroy.txt",      //3
        "../test/test-close.txt",               //4
        "../test/test-buffer-load.txt"          //5
    };

    std::string OUT[] = {
        "../test/test_fs_overflow_output.txt",
        "../test/test_end_of_file_output.txt",
        "../test/output-sample.txt",
        "../test/output-destroy.txt",
        "../test/output-close.txt",
        "../test/output-buffer-load.txt"
    };

    FS::Controller controller("saving.bin");
    controller.reset();
    std::ifstream in(IN[5]);
    std::ofstream out(OUT[5]);
    controller.work(in, out);
    in.close();
    out.close();
    return 0;
}