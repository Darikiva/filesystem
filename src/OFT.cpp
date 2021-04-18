#include "Descriptors.hpp"
#include "OFT.hpp"
#include <iostream>

namespace FS {

OFT::OFT(IOSystem& iosystem)
    : iosystem{iosystem}
    , first_use{true}
{
}

const OFTEntry& OFT::get(std::size_t index)
{
    if (first_use)
    {
        for(int i = 0; i < 5; i++) {
            data[i] = OFTEntry(-1);
        }
        first_use = false;
    }
    return data[index];
}

void OFT::set(size_t index, const OFTEntry& value)
{
    if (first_use)
    {
        for(int i = 0; i < 5; i++) {
            data[i] = OFTEntry(-1);
        }
        first_use = false;
    }
    data[index] = value;
}

void OFT::reset()
{
    for(int i = 0; i < 5; i++) {
        data[i] = OFTEntry(-1);
    }
}

OFT::~OFT()
{
}
} // namespace FS