#include "lib/svr/server_options.h"
namespace dry
{
namespace svr
{
void ServerOptions::ListenOn(const uint16_t port)
{
    // TODO:
    _listenAddress.Assign("127.0.0.1", port);
}

}  // end namespace svr
}  // namespace dry
