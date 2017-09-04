#pragma once
#include "lib/net/inet_address.h"

namespace dry
{
namespace net
{
class Socket
{
public:
    static Socket CreateTcpStream();

    // Invalid fd is ok
    explicit Socket(const int fd) : _fd(fd) {}
    Socket(Socket&& that)
    {
        _fd      = that._fd;
        that._fd = -1;
    }

    // fd maybe not valid (>=0 for valid)
    int Fd() const { return _fd; }

    int ReuseAddr();
    int ReusePort();
    int Listen();
    int Bind(const InetAddress& address);
    Socket Accept();

    ~Socket();

private:
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    int _fd;
};

}  // end namespace net
}  // namespace dry
