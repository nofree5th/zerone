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
    void Start();
};

}  // end namespace svr
}  // namespace dry
