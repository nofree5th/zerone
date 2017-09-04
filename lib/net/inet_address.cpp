#include "lib/net/inet_address.h"
#include <arpa/inet.h>
#include "lib/log/logger.h"

namespace dry
{
namespace net
{
InetAddress::InetAddress(const std::string& ip, uint16_t port) : _addr{0}
{
    _addr.sin_family = AF_INET;
    _addr.sin_port   = htons(port);
    if (::inet_pton(AF_INET, ip.c_str(), &_addr.sin_addr) <= 0)
    {
        DRY_LOG_ERROR("Invalid ip: %s", ip.c_str());
    }
}

std::string InetAddress::ToString() const
{
    return "";
}

}  // end namespace net
}  // namespace dry
