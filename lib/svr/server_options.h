#pragma once
#include <cinttypes>
#include "lib/net/inet_address.h"
namespace dry
{
namespace svr
{
class ServerOptions
{
public:
    void SetConcurrency(const uint32_t threadCount, const uint32_t coroutineCount)
    {
        _threadCount    = threadCount;
        _coroutineCount = coroutineCount;
    }
    void ListenOn(const uint16_t port);

    const uint32_t ThreadCount() const { return _threadCount; }
    const uint32_t CoroutineCount() const { return _coroutineCount; }
    const ::dry::net::InetAddress& ListenAddress() const { return _listenAddress; }

private:
    uint32_t _threadCount    = 0;
    uint32_t _coroutineCount = 0;
    ::dry::net::InetAddress _listenAddress;
};

}  // end namespace svr
}  // namespace dry
