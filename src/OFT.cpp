#include "OFT.hpp"

#include <iostream>

#include "Descriptors.hpp"

namespace FS {

OFT::OFT(IOSystem* iosystem)
    : iosystem{iosystem}
    , first_use{true}
{
}

IOSystem* OFT::getIoSystem() const
{
    return iosystem;
}

OFTEntry OFT::get(std::size_t index)
{
    if (first_use)
    {
        for (int i = 0; i < 5; i++)
        {
            data.push_back(emptyOFTEntry);
        }
        first_use = false;
    }
    return data[index];
}

void OFT::set(size_t index, const OFTEntry& value)
{
    if (first_use)
    {
        for (int i = 0; i < 5; i++)
        {
            data.push_back(emptyOFTEntry);
        }
        first_use = false;
    }
    data[index] = value;
}

void OFT::reset()
{
    for (int i = 0; i < 5; i++)
    {
        data.push_back(emptyOFTEntry);
    }
}

OFT::~OFT() {}
} // namespace FS