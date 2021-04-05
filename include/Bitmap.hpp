#pragma once

#include "Disk.hpp"
#include "IOSystem.hpp"

namespace FS {

class Bitmap
{
public:
    Bitmap(IOSystem& iosystem);
    ~Bitmap();
    bool get(std::size_t index);
    void set(std::size_t index, bool value);
    inline size_t size() const { return data.size(); }
    void reset();

private:
    std::bitset<Disk::bitmap_size_bits> data;
    IOSystem& iosystem;
    bool first_use;
    void loadBitmap();
    void unloadBitmap();
};

} // namespace FS