#include <fstream>
#include <iostream>
#include <sstream>

#include "Controller.hpp"

void sample() {
    FS::Controller controller("saving.bin");

    //comment this after first run
//    controller.reset();
//    std::ifstream in("../test/test-sample-before-restore.txt");
//    std::ofstream out("../test/output/output-sample-before.txt");

    //uncomment this after first run
    std::ifstream in("../test/test-sample-after-restore.txt");
    std::ofstream out("../test/output/output-sample-after.txt");
    controller.work(in, out);

    in.close();
    out.close();
}

int main()
{
    std::string IN[] = {
        "../test/test-fs-overflow.txt",             // 0
        "../test/test-end-of-file.txt",             // 1
        "../test/test-sample.txt",                  // 2
        "../test/test-destroy.txt",                 // 3
        "../test/test-close.txt",                   // 4
        "../test/test-buffer-load.txt",             // 5
        "../test/test-buffer-block-changing.txt",   // 6
        "../test/test-rewrite.txt"
      //  "../test/test-fs-overflow.txt"              // 7
    };

    std::string OUT[] = {
        "../test/output/output-test-fs-overflow.txt",
        "../test/output/output-test-end-of-file.txt",
        "../test/output/output-sample.txt",
        "../test/output/output-destroy.txt",
        "../test/output/output-close.txt",
        "../test/output/output-buffer-load.txt",
        "../test/output/output-test-buffer-block-changing.txt",
        "../test/output/output-rewrite.txt"
        //"../test/output/output-test-fs-overflow.txt"
    };

    FS::Controller controller("saving.bin");

    sample();
//    for (int i = 0; i < sizeof(IN) / sizeof(*IN); i++)
//    {
//        controller.reset();
//        std::ifstream in(IN[i]);
//        std::ofstream out(OUT[i]);
//        controller.work(in, out);
//
//        in.close();
//        out.close();
//    }
    return 0;
}