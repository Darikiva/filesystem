#pragma once

#include <vector>

#include "Disk.hpp"
#include "IOSystem.hpp"
#include "OFTEntry.hpp"

namespace FS {

class OFT
{
public:
    OFT(IOSystem* iosystem);
    ~OFT();
    OFTEntry* get(std::size_t index);
    void set(size_t index, const OFTEntry& value);
    void reset();
    inline size_t size() const { return data.size(); }
    IOSystem* getIoSystem() const;
    OFTEntry emptyOFTEntry = OFTEntry(Entity::FileDescriptor{0, {-1, -1, -1}}, -1, iosystem);

private:
    std::vector<OFTEntry> data;
    bool first_use;
    IOSystem* iosystem;

public:
};

} // namespace FS