#pragma once
#include "boost/core/noncopyable.hpp"
#include "lib/core.h"

namespace dry
{
namespace svr
{
class TcpServer : boost::noncopyable
{
public:
    void Start();
};

}  // end namespace svr
}  // namespace dry
