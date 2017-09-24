#include "lib/bytes/buffer.h"
#include <cstring>

namespace dry
{
namespace bytes
{
void Buffer::Reset()
{
    _readPos  = 0;
    _writePos = 0;
}

const char* Buffer::ReadPtr(size_t* len)
{
    if (len != nullptr) *len = _writePos - _readPos;
    return &_buf[_readPos];
}

void Buffer::Write(size_t bytesCount)
{
    _writePos = std::min(_writePos + bytesCount, _buf.size());
}

char* Buffer::WritePtr(const size_t len)
{
    ensureWriteBytes(len);
    return &_buf[_writePos];
}

void Buffer::Read(const size_t bytesCount)
{
    _readPos = std::min(_readPos + bytesCount, _writePos);
}

void Buffer::ensureWriteBytes(const size_t newBytesCount)
{
    const auto newSize = newBytesCount + _writePos;
    if (newSize <= _buf.size()) return;

    _buf.resize(newSize);
}

}  // end namespace bytes
}  // end namespace dry
