#pragma once
#include <cinttypes>
#include "boost/core/noncopyable.hpp"

struct stCoRoutine_t;

namespace dry
{
namespace svr
{
class ServerThread;
class Acceptor : boost::noncopyable
{
public:
    Acceptor(ServerThread* scheduel, const uint32_t acceptorId);

private:
    static void* runAcceptor(void* pThis);
    int run();

private:
    uint32_t _acceptorId;
    ServerThread* _scheduel;
    stCoRoutine_t* _co;
};

}  // end namespace svr
}  // namespace dry
