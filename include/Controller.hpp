#pragma once

#include <memory>

#include "Disk.hpp"
#include "FileSystem.hpp"
#include "IOSystem.hpp"

namespace FS {

class Controller
{
public:
    Controller(std::string save_name);
    void reset();
    std::unique_ptr<Disk> disk;
private:
    std::unique_ptr<IOSystem> iosystem;
    std::unique_ptr<FileSystem> filesystem;
};

} // namespace FS