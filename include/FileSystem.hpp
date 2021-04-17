#pragma once

#include <bitset>
#include <vector>

#include "Bitmap.hpp"
#include "Descriptors.hpp"
#include "IOSystem.hpp"

namespace FS {

class FileSystem
{
public:
    FileSystem(IOSystem& iosystem);
    void reset();
private:
    IOSystem& iosystem;
    Bitmap bitmap;
    Descriptors descriptors;
};

} // namespace FS