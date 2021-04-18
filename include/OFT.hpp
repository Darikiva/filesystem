#pragma once

#include <vector>

#include "Disk.hpp"
#include "IOSystem.hpp"

namespace FS {

class OFT
{
public:
    OFT(IOSystem& iosystem);
    ~OFT();
    const OFTEntry& get(std::size_t index);
    void set(size_t index, const OFTEntry& value);
    void reset();
    inline size_t size() const { return 5; }

private:
    OFTEntry data[5];
    bool first_use;
    IOSystem& ioSystem;
};

} // namespace FS