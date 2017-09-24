#pragma once
#include <cinttypes>
#include <cstdlib>
#include <vector>

namespace dry
{
namespace bytes
{
// A Buffer is a variable-sized buffer of bytes with Read and Write methods.
class Buffer
{
public:
    Buffer(const size_t initSize = 1024) : _buf(initSize, 0) {}

    const char* ReadPtr(size_t* len);
    char* WritePtr(const size_t len);
    void Write(const size_t len);
    void Read(const size_t bytesCount);
    void Reset();

private:
    void ensureWriteBytes(const size_t newBytesCount);

private:
    std::vector<char> _buf;
    // read at &buf[readPos], write at &buf[writePos]
    size_t _readPos  = 0;
    size_t _writePos = 0;
};

}  // end namespace bytes
}  // end namespace dry
