#include "Controller.hpp"

namespace FS {
Controller::Controller(std::string save_name)
{
    disk = std::make_unique<Disk>(save_name);
    iosystem = std::make_unique<IOSystem>(*disk);
    filesystem = std::make_unique<FileSystem>(*iosystem);
}

void Controller::reset()
{
    filesystem->reset();
}
} // namespace FS