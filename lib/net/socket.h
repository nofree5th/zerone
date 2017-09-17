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

    // Invalid() is ok
    Socket() : _fd(-1) {}
    explicit Socket(const int fd) : _fd(fd) {}
    Socket(Socket&& that)
    {
        _fd      = that._fd;
        that._fd = -1;
    }
    Socket& operator=(Socket&& that);

    bool Valid() const { return _fd >= 0; }
    bool Invalid() const { return !Valid(); }
    // -> invalid
    void Release() { tryRelease(); }
    // fd maybe not valid
    int Fd() const { return _fd; }

    int ReuseAddr();
    int ReusePort();
    int Listen();
    int Bind(const InetAddress& address);
    // O_NONBLOCK | O_NDELAY | FD_CLOEXEC
    int SetFlags(const int extraFlags);
    Socket Accept();

    ~Socket();

private:
    void tryRelease();

private:
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    int _fd;
};

}  // end namespace net
}  // namespace dry
