#include "FileSystem.hpp"

namespace FS {

FileSystem::FileSystem(IOSystem& iosystem)
    : iosystem{iosystem}
    , bitmap{iosystem}
    , descriptors{iosystem}
{
}

void FileSystem::reset()
{
    bitmap.reset();
    descriptors.reset();
}
} // namespace FS