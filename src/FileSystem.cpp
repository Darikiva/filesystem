#include "FileSystem.hpp"

namespace FS {

FileSystem::FileSystem(IOSystem& iosystem)
    : iosystem{iosystem}
    , bitmap{iosystem}
    , descriptors{iosystem}
    , directories{iosystem}
    , oft{&iosystem}
{
}

void FileSystem::reset()
{
    bitmap.reset();
    descriptors.reset();
    directories.reset();
    oft.reset();
}

/**
 *
 * 1) find a free file descriptor (scan ldisk[0] through ldisk[k-1])
 * 2) find free entry in the directory_entries
 * 3) verify that the file does not already exist -> (yes) return error status
 * 4) enter file_name and descriptor_index into directory entry
 * 5) return status
 */
Status FileSystem::create(const std::string& file_name)
{
    std::int8_t desc_index = -1;
    for (int i = 0; i < descriptors.size(); i++)
    {
        auto data = descriptors.get(i);
        if (data.indexes[0] == -1)
        {
            desc_index = i;
            break;
        }
    }

    if (desc_index == -1)
    {
        return Status::Error;
    }

    int dir_index = -1;
    for (auto i = directories.size() - 1; i >= 0; i--)
    {
        auto data = directories.get(i);
        if (data.descriptor_index == -1)
        {
            dir_index = i;
        }
        else
        {
            bool same_names = true;
            for (int j = 0; j < 4; j++)
            {
                if (data.file_name[j] != file_name.at(j))
                {
                    same_names = false;
                }
            }
            if (same_names)
            {
                return Status::Error;
            }
        }
    }

    if (dir_index == -1)
    {
        return Status::Error;
    }

    // free blocks

    std::int8_t free_blocks_indexes[3] = {-1, -1, -1};
    int number_of_found = 0;
    for (int i = 0; i < bitmap.size(); i++)
    {
        if (number_of_found == 3)
        {
            break;
        }
        if (!bitmap.get(i))
        {
            free_blocks_indexes[number_of_found] = i;
            ++number_of_found;
        }
    }

    if (number_of_found != 3)
    {
        return Status::Error;
    }

    Entity::FileDescriptor directory_desc = {0, *free_blocks_indexes};
    descriptors.set(desc_index, directory_desc);

    char name_char[4]{file_name.at(0), file_name.at(1), file_name.at(2), file_name.at(3)};
    Entity::DirectoryEntry directory_entry = {desc_index, *name_char};
    directories.set(dir_index, directory_entry);
    return Status::Success;
}

/**
 *
 * 1) find file_descriptor in directory
 * 2) remove directory entry
 * 3) update bitmap to reflect freed blocks
 * 4) free file descriptor
 * 5) return status
 */
Status FileSystem::destroy(const std::string& file_name)
{
    int desc_index = -1;
    for (auto i = 0; i < directories.size(); i++)
    {
        auto data = directories.get(i);
        bool same_names = true;
        for (int j = 0; j < 4; j++)
        {
            if (data.file_name[j] != file_name.at(j))
            {
                same_names = false;
            }
        }
        if (same_names)
        {
            desc_index = data.descriptor_index;
            char empty_name[4] = {};
            Entity::DirectoryEntry empty_entry = {-1, *empty_name};
            directories.set(i, empty_entry);
            break;
        } else {
            return Status::Error;
        }
    }

    auto desc = descriptors.get(desc_index);
    for (signed char index : desc.indexes)
    {
        bitmap.set(index, false);
    }

    Entity::FileDescriptor empty_desc = {0, {-1, -1, -1}};
    descriptors.set(desc_index, empty_desc);
    return Status::Success;
}

/**
 *
 * 1) search directory to find the index of the file_descriptor +
 * 2) allocate a free OFT entry +
 * 3) fill in curr_pos and file_descriptor_index +
 * 4) read the first block of file into the buffer[] +
 * 5) return OFT index (error status) +
 */
size_t FileSystem::open(const std::string& file_name) // TODO ENUM VS RETURN OFT INDEX
{
    int desc_index = -1;
    for (auto i = 0; i < directories.size(); i++)
    {
        auto data = directories.get(i);
        bool same_names = true;
        for (int j = 0; j < 4; j++)
        {
            if (data.file_name[j] != file_name.at(j))
            {
                same_names = false;
            }
        }
        if (same_names)
        {
            desc_index = data.descriptor_index;
            break;
        }
    }

    int oft_index = -1;
    for (int i = 0; i < oft.size(); i++)
    {
        if (oft.get(i).isEmpty())
        {
            oft_index = i;
        }
    }

    if (oft_index == -1)
    {
        // throw some exception
        return -1;
    }

    oft.set(oft_index, OFTEntry(descriptors.get(desc_index), oft.getIoSystem()));

    return oft_index;
}

/**
 *
 * 1) write buffer to disk +
 * 2) update file_length of descriptor TODO
 * 3) free OWT entry +
 */
void FileSystem::close(size_t index)
{
    oft.get(index).onClose();
    // TODO file_length
    oft.set(index, oft.emptyOFTEntry);
}

void FileSystem::read(size_t index, char* mem_area, size_t count) {}

void FileSystem::write(size_t index, char* mem_area, size_t count) {}

void FileSystem::lseek(size_t index, size_t pos)
{
    if (pos < 0 || pos > Disk::BLOCK_SIZE * 3 - 1)
    {
        // throw some exception
        return;
    }
    oft.get(index).setPosition(pos);
}

std::unordered_map<std::string, size_t> FileSystem::directory()
{
    return std::unordered_map<std::string, size_t>();
}

} // namespace FS