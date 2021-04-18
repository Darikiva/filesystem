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

void FileSystem::create(const std::string& file_name)
{
    // find a free file descriptor (scan ldisk[0] through ldisk[k-1])
    // find free entry in the directory_entries
    // verify that the file does not already exist -> (yes) return error status
    // enter file_name and descriptor_index into directory entry
    // return status
}

void FileSystem::destroy(const std::string& file_name)
{
    // find file_descriptor in directory
    // remove directory entry
    // update bitmap to reflect freed blocks
    // free file descriptor
    // return status
}

size_t FileSystem::open(const std::string& file_name)
{
    // search directory to find the index of the file_descriptor
    // allocate a free OFT entry
    // fill in curr_pos and file_descriptor_index
    // read the first block of file into the buffer[]
    // return OFT index (error status)

    return 0;
}

void FileSystem::close(size_t index)
{
    // write buffer to disk
    // update file_length if descriptor
    // free OWT entry
    // return status
}

void FileSystem::read(size_t index, char* mem_area, size_t count) {}

void FileSystem::write(size_t index, char* mem_area, size_t count) {}

void FileSystem::lseek(size_t index, size_t pos)
{
    /* - ????
if (pos == 0) {
    reset();
}
    */
}

std::unordered_map<std::string, size_t> FileSystem::directory()
{
    return std::unordered_map<std::string, size_t>();
}

} // namespace FS