#pragma once
#include <netinet/in.h>
#include <cinttypes>
#include <string>

namespace dry
{
namespace net
{
class InetAddress
{
public:
    InetAddress() : _addr{0} {}
    InetAddress(const std::string& ip, uint16_t port);

    void Assign(const std::string& ip, uint16_t port);

    const sockaddr_in& GetAddr() const { return _addr; }
    std::string ToString() const;

private:
    struct sockaddr_in _addr;
};

}  // end namespace net
}  // namespace dry
