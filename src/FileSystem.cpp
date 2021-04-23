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
    for (size_t i = 0; i < descriptors.size(); i++)
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
        return Status::NoSpace;
    }

    int dir_index = -1;
    for (size_t i = directories.size() - 1; i >= 0; i--)
    {
        auto data = directories.get(i);
        if (data.descriptor_index == -1)
        {
            dir_index = i;
        }
        else
        {
            bool same_names = true;
            for (size_t j = 0; j < 4; j++)
            {
                if (data.file_name[j] != file_name.at(j))
                {
                    same_names = false;
                }
            }
            if (same_names)
            {
                return Status::AlreadyExists;
            }
        }
    }

    if (dir_index == -1)
    {
        return Status::NoSpace;
    }

    // free blocks
    std::int8_t free_blocks_indexes[3] = {-1, -1, -1};
    int number_of_found = 0;
    for (size_t i = 0; i < bitmap.size(); i++)
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
        return Status::NoSpace;
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
    bool same_names = true;
    for (size_t i = 0; i < directories.size(); i++)
    {
        auto data = directories.get(i);
        for (size_t j = 0; j < 4; j++)
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
        }
    }
    if (!same_names)
    {
        return Status::NotFound;
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
std::pair<Status, size_t> FileSystem::open(const std::string& file_name)
{
    // search directories for file and get descriptor index
    int desc_index = -1;
    for (size_t i = 0; i < directories.size(); i++)
    {
        auto data = directories.get(i);
        bool same_names = true;
        for (size_t j = 0; j < 4; j++)
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

    if (desc_index == -1)
    {
        return std::pair<Status, size_t>(Status::NotFound, -1);
    }

    // find free oft
    int oft_index = -1;
    for (size_t i = 0; i < oft.size(); i++)
    {
        if (oft.get(i)->isEmpty())
        {
            oft_index = i;
        }
    }

    if (oft_index == -1)
    {
        return std::pair<Status, size_t>(Status::NoSpace, -1);
    }

    oft.set(oft_index, OFTEntry(descriptors.get(desc_index), desc_index, oft.getIoSystem()));

    return std::pair<Status, size_t>(Status::Success, oft_index);
}

/**
 *
 * 1) write buffer to disk +
 * 2) update file_length of descriptor +
 * 3) free OWT entry +
 */
Status FileSystem::close(size_t index)
{
    OFTEntry* oft_entry = oft.get(index);

    if (oft_entry == nullptr || oft_entry->isEmpty())
    {
        return Status::NotFound;
    }

    oft_entry->onClose();
    descriptors.set(oft_entry->getDescriptorIndex(), oft_entry->getDescriptor());
    oft.set(index, oft.emptyOFTEntry);
    return Status::Success;
}

std::pair<Status, int> FileSystem::read(size_t index, char* mem_area, int count)
{
    OFTEntry* oft_entry = oft.get(index);

    if (oft_entry == nullptr || oft_entry->isEmpty())
    {
        return std::pair<Status, int>(Status::NotFound, 0);
    }

    const char* read_chars = oft_entry->readFromBuffer(count);
    for (size_t i = 0; i < sizeof(read_chars); i++)
    {
        if (sizeof(mem_area) == i)
        {
            return std::pair<Status, int>(Status::OutputArrIndexOutOfBounds, i);
            break;
        }

        mem_area[i] = read_chars[i];
    }

    if (sizeof(read_chars) != count)
    {
        return std::pair<Status, int>(Status::EndOfFile, sizeof(read_chars));
    }
    else
    {
        return std::pair<Status, int>(Status::Success, count);
    }
}

std::pair<Status, int> FileSystem::write(size_t index, char* mem_area, int count)
{
    OFTEntry* oft_entry = oft.get(index);

    if (oft_entry == nullptr || oft_entry->isEmpty())
    {
        return std::pair<Status, int>(Status::NotFound, 0);
    }

    size_t number_of_written = oft_entry->writeToBuffer(mem_area, count);

    if (number_of_written != count)
    {
        return std::pair<Status, int>(Status::EndOfFile, number_of_written);
    }
    else
    {
        return std::pair<Status, int>(Status::Success, count);
    }
}

Status FileSystem::lseek(size_t index, size_t pos)
{
    if (pos < 0 || pos > Disk::BLOCK_SIZE * 3 - 1)
    {
        return Status::PositionOutOfBounds;
    }
    oft.get(index)->setPosition(pos);
    return Status::Success;
}

std::unordered_map<std::string, int8_t> FileSystem::directory()
{
    std::unordered_map<std::string, int8_t> directory_map;
    for(size_t i = 0; i < directories.size(); i++) {
        auto directory = directories.get(i);
        directory_map[directory.file_name] = directory.descriptor_index;
    }
    return directory_map;
}

} // namespace FS