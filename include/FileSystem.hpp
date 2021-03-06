#pragma once

#include <bitset>
#include <unordered_map>
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
    std::pair<Status, int> read(size_t index, char* mem_area, int count);
    std::pair<Status, int> write(size_t index, char* mem_area, int count);
    std::pair<Status, size_t> lseek(size_t index, size_t pos);
    std::unordered_map<std::string, uint8_t> directory();

private:
    IOSystem& iosystem;
    Bitmap bitmap;
    Descriptors descriptors;
    Directories directories;
    OFT oft;

    std::int8_t findFreeBlock();
};

} // namespace FS