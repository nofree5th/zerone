#pragma once
#include <memory>

#include "boost/core/noncopyable.hpp"
#include "lib/error/error.h"
#include "lib/log/logger.h"
#include "lib/svr/server_options.h"

namespace dry
{
namespace svr
{
class ServerThreadImpl;
class ServerThread : boost::noncopyable
{
public:
    ServerThread(const uint32_t id, const ServerOptions& options);
    ~ServerThread();

    void Join();

private:
    ServerThreadImpl* _impl;
};
}  // end namespace svr
}  // namespace dry
