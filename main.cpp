#include <iostream>
#include <sstream>
#include <fstream>

#include "Controller.hpp"


int main()
{
    std::string IN[] = {
        "../test/test-fs-overflow.txt",         //0
        "../test/test-end-of-file.txt",         //1
        "../test/test-sample.txt",              //2
        "../test/test-destroy.txt",             //3
        "../test/test-close.txt",               //4
        "../test/test-buffer-load.txt",          //5
        "../test/test-buffer-block-changing.txt" //6
    };

    std::string OUT[] = {
        "../test/output-test-fs-overflow.txt",
        "../test/output-test-end-of-file.txt",
        "../test/output-sample.txt",
        "../test/output-destroy.txt",
        "../test/output-close.txt",
        "../test/output-buffer-load.txt",
        "../test/output-test-buffer-block-changing.txt"
    };

    FS::Controller controller("saving.bin");
    controller.reset();
    std::ifstream in(IN[6]);
    std::ofstream out(OUT[6]);
    controller.work(in, out);
    in.close();
    out.close();
    return 0;
}