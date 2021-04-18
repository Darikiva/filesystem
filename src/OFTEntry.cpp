#include "OFTEntry.hpp"

namespace FS {

void OFTEntry::setBuffer(char* buffer)
{
    if (isBufferChanged) {
        updateBuffer();
    }

}

void OFTEntry::readBuffer()
{

}

void OFTEntry::updateBuffer()
{
    iosystem.read_block(cur_block, buffer);
}
}; // namespace FS