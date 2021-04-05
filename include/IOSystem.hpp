#pragma once

#include "Disk.hpp"
#include <string>
#include <fstream>
#include <boost/optional.hpp>
#include <bitset>


namespace FS {

class IOSystem
{
public:
    void read_block(std::size_t i, char* p);
    void write_block(std::size_t i, char* p);
    IOSystem(Disk& disk);

private:
    Disk& disk;
};

} // namespace FS