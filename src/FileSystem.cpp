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

std::int8_t FileSystem::findFreeBlock()
{
    std::int8_t free_block_index = -1;
    for (size_t i = 0; i < bitmap.size(); i++)
    {
        if (!bitmap.get(i))
        {
            bitmap.set(i, true);
            free_block_index = Disk::K + i;
            break;
        }
    }
    return free_block_index;
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
    for (int i = directories.size() - 1; i >= 0; i--)
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

    std::int8_t free_block_index = findFreeBlock();

    if (free_block_index == -1)
    {
        return Status::NoSpace;
    }

    Entity::FileDescriptor directory_desc = {0, free_block_index, -1, -1};
    descriptors.set(desc_index, directory_desc);

    char name_char[4]{file_name.at(0), file_name.at(1), file_name.at(2), file_name.at(3)};
    Entity::DirectoryEntry directory_entry = {
        desc_index, name_char[0], name_char[1], name_char[2], name_char[3]};
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
        if (index != -1)
        {
            bitmap.set(index, false);
        }
    }

    Entity::FileDescriptor empty_desc = {0, {-1, -1, -1}};
    descriptors.set(desc_index, empty_desc);

    for (int i = 0; i < oft.size(); i++)
    {
        auto oft_entry = oft.get(i);
        if (!oft_entry->isEmpty() && oft_entry->getDescriptorIndex() == desc_index)
        {
            oft.set(i, oft.emptyOFTEntry);
            break;
        }
    }
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
        if (oft.get(i)->getDescriptorIndex() == desc_index)
        {
            lseek(i + 1, 0);
            return std::pair<Status, size_t>(Status::Success, i + 1);
        }
        if (oft.get(i)->isEmpty())
        {
            oft_index = i;
            break;
        }
    }

    if (oft_index == -1)
    {
        return std::pair<Status, size_t>(Status::NoSpace, -1);
    }

    oft.set(oft_index, OFTEntry(descriptors.get(desc_index), desc_index, oft.getIoSystem()));

    return std::pair<Status, size_t>(Status::Success, oft_index + 1);
}

/**
 *
 * 1) write buffer to disk +
 * 2) update file_length of descriptor +
 * 3) free OWT entry +
 */
Status FileSystem::close(size_t index)
{
    size_t index_in_oft_array = index - 1;
    OFTEntry* oft_entry = oft.get(index_in_oft_array);

    if (oft_entry == nullptr || oft_entry->isEmpty())
    {
        return Status::NotFound;
    }

    oft_entry->onClose();
    descriptors.set(oft_entry->getDescriptorIndex(), oft_entry->getDescriptor());
    oft.set(index_in_oft_array, oft.emptyOFTEntry);
    return Status::Success;
}

std::pair<Status, int> FileSystem::read(size_t index, char* mem_area, int count)
{
    size_t index_in_oft_array = index - 1;
    OFTEntry* oft_entry = oft.get(index_in_oft_array);

    if (oft_entry == nullptr || oft_entry->isEmpty())
    {
        return std::pair<Status, int>(Status::NotFound, 0);
    }

    auto read_result = oft_entry->readFromBuffer(count);
    for (size_t i = 0; i < read_result.second; i++)
    {
        mem_area[i] = read_result.first[i];
    }

    if (read_result.second != count)
    {
        return std::pair<Status, int>(Status::Success, read_result.second);
    }
    else
    {
        return std::pair<Status, int>(Status::Success, count);
    }
}

std::pair<Status, int> FileSystem::write(size_t index, char* mem_area, int count)
{
    size_t index_in_oft_array = index - 1;
    OFTEntry* oft_entry = oft.get(index_in_oft_array);

    if (oft_entry == nullptr || oft_entry->isEmpty())
    {
        return std::pair<Status, int>(Status::NotFound, 0);
    }

    size_t positionAfterWrite = oft_entry->getAbsoluteCurrentPosition() + count;
    for (int i = 1; i < sizeof(oft_entry->getDescriptor().indexes); i++)
    {
        if (positionAfterWrite > Disk::BLOCK_SIZE * i)
        {
            if (oft_entry->getDescriptor().indexes[i] == -1)
            {
                oft_entry->setDescriptorIndex(findFreeBlock(), i);
            }
        }
    }

    size_t number_of_written = oft_entry->writeToBuffer(mem_area, count);
    descriptors.set(oft_entry->getDescriptorIndex(), oft_entry->getDescriptor());

    if (number_of_written != count)
    {
        return std::pair<Status, int>(Status::Success, number_of_written);
    }
    else
    {
        return std::pair<Status, int>(Status::Success, count);
    }
}

std::pair<Status, size_t> FileSystem::lseek(size_t index, size_t pos)
{
    size_t index_in_oft_array = index - 1;

    if (oft.get(index_in_oft_array) == nullptr || oft.get(index_in_oft_array)->isEmpty())
    {
        return {Status::NotFound, -1};
    }

    if (pos < 0 || pos > Disk::BLOCK_SIZE * 3 - 1)
    {
        return {Status::PositionOutOfBounds, -1};
    }
    auto cur_pos = oft.get(index_in_oft_array)->setPosition(pos);
    return {Status::Success, cur_pos};
}

std::unordered_map<std::string, uint8_t> FileSystem::directory()
{
    std::unordered_map<std::string, uint8_t> directory_map;
    for (size_t i = 0; i < directories.size(); i++)
    {
        auto directory = directories.get(i);
        if (directory.descriptor_index != -1)
        {
            std::int8_t descriptor_index = directory.descriptor_index;
            auto file_length = descriptors.get(descriptor_index).file_length;
            directory_map[directory.file_name] = file_length;
        }
    }
    return directory_map;
}

} // namespace FS