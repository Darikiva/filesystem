#pragma once

#include <bitset>
#include <vector>

#include "Bitmap.hpp"
#include "Descriptors.hpp"
#include "Directories.hpp"
#include "Entities.hpp"
#include "EnumMacros.hpp"
#include "IOSystem.hpp"
#include "OFT.hpp"
#include "OFTEntry.hpp"

DECLARE_ENUM(
    Status,
    Success,
    NoSpace,
    NotFound,
    NoFreeDescriptor,
    AlreadyExists,
    OutputArrIndexOutOfBounds,
    EndOfFile,
    PositionOutOfBounds)

namespace FS {

class FileSystem
{
public:
    FileSystem(IOSystem& iosystem);
    void reset();
    Status create(const std::string& file_name);
    Status destroy(const std::string& file_name);
    std::pair<Status, size_t> open(const std::string& file_name);
    Status close(size_t index);
    std::pair<Status, size_t> read(size_t index, char* mem_area, size_t count);
    std::pair<Status, size_t> write(size_t index, char* mem_area, size_t count);
    Status lseek(size_t index, size_t pos);
    std::unordered_map<std::string, size_t> directory();

private:
    IOSystem& iosystem;
    Bitmap bitmap;
    Descriptors descriptors;
    Directories directories;
    OFT oft;
};

} // namespace FS