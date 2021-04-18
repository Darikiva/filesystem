#pragma once

#include <bitset>
#include <vector>

#include "Bitmap.hpp"
#include "Descriptors.hpp"
#include "Directories.hpp"
#include "IOSystem.hpp"

namespace FS {

class FileSystem
{
public:
    FileSystem(IOSystem& iosystem);
    void reset();

    // created by Sanya
    void create(const std::string& file_name);
    void destroy(const std::string& file_name);
    size_t open(
        const std::string& file_name); // return an index value for read, write, lseek, close
    void close(size_t index);
    void read(size_t index, char* mem_area, size_t count);
    void write(size_t index, char* mem_area, size_t count);
    void lseek(size_t index, size_t pos);
    std::unordered_map<std::string, size_t> directory();

private:
    IOSystem& iosystem;
    Bitmap bitmap;
    Descriptors descriptors;
    Directories directories;
};

} // namespace FS