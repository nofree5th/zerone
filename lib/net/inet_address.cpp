#include "lib/net/inet_address.h"
#include <arpa/inet.h>
#include <strings.h>  // bzero
#include "lib/log/logger.h"

namespace dry
{
namespace net
{
InetAddress::InetAddress(const std::string& ip, uint16_t port) : _addr{0}
{
    Assign(ip, port);
}

void InetAddress::Assign(const std::string& ip, uint16_t port)
{
    if (::inet_pton(AF_INET, ip.c_str(), &_addr.sin_addr) <= 0)
    {
        DRY_LOG_ERROR("Invalid ip: %s", ip.c_str());
        bzero(&_addr, sizeof(_addr));
        return;
    }
    _addr.sin_family = AF_INET;
    _addr.sin_port   = htons(port);
}

std::string InetAddress::ToString() const
{
    return std::string() + ::inet_ntoa(_addr.sin_addr) + ":" + std::to_string(ntohs(_addr.sin_port));
}

}  // end namespace net
}  // namespace dry
