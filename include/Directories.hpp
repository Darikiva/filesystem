#pragma once

#include <vector>

#include "Disk.hpp"
#include "Entities.hpp"
#include "IOSystem.hpp"

namespace FS {

class Directories
{
public:
    Directories(IOSystem& iosystem);
    ~Directories();
    Entity::DirectoryEntry& get(std::size_t index);
    void set(size_t index, const Entity::DirectoryEntry& value);
    void reset();
    inline size_t size() const { return Disk::BLOCK_SIZE / directory_size; }

private:
    std::vector<Entity::DirectoryEntry> data;
    IOSystem& iosystem;
    bool first_use;
    void loadDirectories();
    void unloadDirectories();
    void unloadDirectory(size_t index);
    static constexpr size_t directory_size = sizeof(Entity::DirectoryEntry);
};

} // namespace FS