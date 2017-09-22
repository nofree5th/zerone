#pragma once
namespace dry
{
namespace svr
{
struct ServerOptions
{
    uint16_t port;

    uint32_t threadCount;
    uint32_t coroutineCount;
};

}  // end namespace svr
}  // namespace dry
