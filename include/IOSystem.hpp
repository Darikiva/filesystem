#pragma once

#include <bitset>
#include <boost/optional.hpp>
#include <fstream>
#include <string>

#include "Disk.hpp"

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