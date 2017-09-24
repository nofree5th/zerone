#pragma once
#include "boost/core/noncopyable.hpp"
#include "lib/bytes/buffer.h"
#include "lib/net/socket.h"
#include "lib/svr/server_options.h"

namespace dry
{
namespace svr
{
class ConnectionHandler : boost::noncopyable
{
public:
    // options should live long then ConnectionHandler
    explicit ConnectionHandler(const ServerOptions& options) : _options(options) {}
    void AttachNewConnection(::dry::net::Socket&& newConnection);
    inline bool HasConnection() const { return _socket.Valid(); }
    void Processing();

private:
    ::dry::net::Socket _socket;
    const ServerOptions& _options;
    ::dry::bytes::Buffer _buf;
};

}  // end namespace svr
}  // namespace dry
