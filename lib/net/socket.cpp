#include "lib/net/socket.h"

#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include "lib/error/error.h"
#include "lib/log/logger.h"

namespace dry
{
namespace net
{
Socket Socket::CreateTcpStream()
{
    const int fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd < 0)
    {
        DRY_LOG_ERROR("Create socket failed: %d", errno);
    }
    return Socket(fd);
}

Socket& Socket::operator=(Socket&& that)
{
    if (this != &that)
    {
        tryRelease();
        std::swap(_fd, that._fd);
    }
    return *this;
}

Socket Socket::Accept()
{
    struct sockaddr_in peerAddr;
    socklen_t peerAddrLen = sizeof(peerAddr);
    const int connFd      = ::accept(_fd, (struct sockaddr*)&peerAddr, &peerAddrLen);
    if (connFd < 0)
    {
        DRY_LOG_ERROR("Accept socket failed: %d", errno);
    }

    return Socket(connFd);
}

void Socket::tryRelease()
{
    if (_fd >= 0)
    {
        close(_fd);
        _fd = -1;
    }
}

Socket::~Socket()
{
    tryRelease();
}

int Socket::ReuseAddr()
{
    const int optval = 1;
    const int ret    = ::setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &optval, static_cast<socklen_t>(sizeof optval));
    DRY_RETURN_IF_F(ret < 0, dry::error::EC_BAD_LUCK, "Reuse addr failed: %d, fd: %d", errno, _fd);

    return 0;
}

int Socket::ReusePort()
{
    const int optval = 1;
    const int ret    = ::setsockopt(_fd, SOL_SOCKET, SO_REUSEPORT, &optval, static_cast<socklen_t>(sizeof optval));
    DRY_RETURN_IF_F(ret < 0, dry::error::EC_BAD_LUCK, "Reuse port failed: %d", errno);

    return 0;
}

int Socket::SetFlags(const int extraFlags)
{
    int flags = fcntl(_fd, F_GETFL, 0);
    DRY_RETURN_IF_F(flags == -1, dry::error::EC_BAD_LUCK, "GETFL failed: %d, fd: %d", errno, _fd);

    flags |= extraFlags;

    const int ret = fcntl(_fd, F_SETFL, flags);
    DRY_RETURN_IF_F(ret == -1, dry::error::EC_BAD_LUCK, "SETFL failed: %d, fd: %d", errno, _fd);
    return ret;
}

int Socket::Listen()
{
    const int ret = ::listen(_fd, SOMAXCONN);
    DRY_RETURN_IF_F(ret < 0, dry::error::EC_BAD_LUCK, "Listen failed: %d, fd: %d", errno, _fd);

    return 0;
}

int Socket::Bind(const InetAddress& addr)
{
    const int ret = ::bind(_fd, (const struct sockaddr*)(&addr.GetAddr()), sizeof(addr.GetAddr()));
    DRY_RETURN_IF_F(
        ret < 0, dry::error::EC_BAD_LUCK, "Bind to addr: %s failed: %d, fd: %d", addr.ToString().c_str(), errno, _fd);

    return 0;
}

}  // end namespace net
}  // namespace dry
