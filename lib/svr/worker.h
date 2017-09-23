#pragma once
#include "boost/core/noncopyable.hpp"
#include "lib/svr/connection_handler.h"

struct stCoRoutine_t;

namespace dry
{
namespace svr
{
class ServerThread;
class Worker : boost::noncopyable
{
public:
    Worker(ServerThread* scheduel, const uint32_t workerId);

    void HandleNewConnection(::dry::net::Socket&& newConnection);

private:
    static void* runWorker(void* pThis);
    void run();

private:
    ServerThread* _scheduel;
    uint32_t _workerId;
    stCoRoutine_t* _co;
    ConnectionHandler _connectionHandler;
};

}  // end namespace svr
}  // namespace dry
