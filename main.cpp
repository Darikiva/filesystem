#include <fstream>
#include <iostream>
#include <sstream>

#include "Controller.hpp"

int main()
{
    std::string IN[] = {
        "../test/test-fs-overflow.txt",             // 0
        "../test/test-end-of-file.txt",             // 1
        "../test/test-sample.txt",                  // 2
        "../test/custom-test-destroy.txt",          // 3
        "../test/test-close.txt",                   // 4
        "../test/test-buffer-load.txt",             // 5
        "../test/test-buffer-block-changing.txt",   // 6
        "../test/test-rewrite.txt"                  // 7
    };

    std::string OUT[] = {
        "../test/output/output-test-fs-overflow.txt",
        "../test/output/output-test-end-of-file.txt",
        "../test/output/output-sample.txt",
        "../test/output/output-destroy.txt",
        "../test/output/output-close.txt",
        "../test/output/output-buffer-load.txt",
        "../test/output/output-test-buffer-block-changing.txt",
        "../test/output/output-rewrite.txt"};

    FS::Controller controller("saving.bin");
    for (int i = 0; i < sizeof(IN) / sizeof(*IN); i++)
    {
        controller.reset();
        std::ifstream in(IN[i]);
        std::ofstream out(OUT[i]);
        controller.work(in, out);

        in.close();
        out.close();
    }
    return 0;
}