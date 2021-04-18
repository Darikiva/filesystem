#include "FileSystem.hpp"

namespace FS {

FileSystem::FileSystem(IOSystem& iosystem)
    : iosystem{iosystem}
    , bitmap{iosystem}
    , descriptors{iosystem}
    , directories{iosystem}
{
}

void FileSystem::reset()
{
    bitmap.reset();
    descriptors.reset();
}

/**
 *
 * 1) find a free file descriptor (scan ldisk[0] through ldisk[k-1])
 * 2) find free entry in the directory_entries
 * 3) verify that the file does not already exist -> (yes) return error status
 * 4) enter file_name and descriptor_index into directory entry
 * 5) return status
 */
void FileSystem::create(const std::string& file_name)
{
    std::int8_t desc_index = -1;
    for (int i = 0; i < descriptors.size(); i++) {
        auto data = descriptors.get(i);
        if (data.indexes[0] == -1) {
            desc_index = i;
            break;
        }
    }

    if (desc_index == -1) {
        //throw some exception
        return;
    }

    int dir_index = -1;
    for (auto i = directories.size() - 1; i >= 0 ; i--) {
        auto data = directories.get(i);
        if (data.descriptor_index == -1) {
            dir_index = i;
        } else {
            bool same_names = true;
            for (int j = 0; j < 4; j++) {
                if (data.file_name[j] != file_name.at(j)) {
                    same_names = false;
                }
            }
            if (same_names) {
                //throw error
                return;
            }
        }
    }

    if (dir_index == -1) {
        //throw some exception
        return;
    }

    //free blocks

    std::int8_t free_blocks_indexes[3] = {-1, -1, -1};
    int number_of_found = 0;
    for (int i = 0; i < bitmap.size(); i++) {
        if (number_of_found == 3) {
            break;
        }
        if (!bitmap.get(i)) {
            free_blocks_indexes[number_of_found] = i;
            ++number_of_found;
        }
    }

    if (number_of_found != 3) {
        //throw exception
        return;
    }

    Entity::FileDescriptor directory_desc = {0, *free_blocks_indexes};
    descriptors.set(desc_index, directory_desc);

    char name_char[4] {file_name.at(0), file_name.at(1),
                        file_name.at(2), file_name.at(3)};
    Entity::DirectoryEntry directory_entry = {desc_index, *name_char};
    directories.set(dir_index, directory_entry);
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