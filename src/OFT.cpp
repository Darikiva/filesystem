#include "OFT.hpp"

#include <iostream>

#include "Descriptors.hpp"

namespace FS {

OFT::OFT(IOSystem* iosystem)
    : iosystem{iosystem}
    , first_use{true}
    , emptyOFTEntry(OFTEntry(Entity::FileDescriptor{0, {-1, -1, -1}}, -1, iosystem))
{
}

IOSystem* OFT::getIoSystem() const
{
    return iosystem;
}

OFTEntry* OFT::get(std::size_t index)
{
    if (first_use)
    {
        for (int i = 0; i < 5; i++)
        {
            data.push_back(emptyOFTEntry);
        }
        first_use = false;
    }
    if (index < data.size())
    {
        return &data[index];
    }
    return nullptr;
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
    data.clear();
    for (int i = 0; i < 5; i++)
    {
        data.push_back(emptyOFTEntry);
    }
}

OFT::~OFT() {}
} // namespace FS