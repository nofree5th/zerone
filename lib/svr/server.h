#pragma once
#include "boost/core/noncopyable.hpp"
#include "lib/svr/server_options.h"

namespace dry
{
namespace svr
{
class Server : boost::noncopyable
{
public:
    Server(const ServerOptions& options) : _options(options) {}
    int StartUntilWorldOfEnd();

private:
    ServerOptions _options;
};

}  // end namespace svr
}  // namespace dry
