#include "lib/svr/acceptor.h"

#include <fcntl.h>
#include <sys/socket.h>

#include "co_routine.h"
#include "lib/svr/server_thread.h"
namespace dry
{
namespace svr
{
// libco
// TODO:
int co_accept(int fd, struct sockaddr* addr, socklen_t* len);

void* Acceptor::runAcceptor(void* pThis)
{
    ::co_enable_hook_sys();
    (void)reinterpret_cast<Acceptor*>(pThis)->run();
    // ret ignored
    return nullptr;
}

Acceptor::Acceptor(ServerThread* scheduel, const uint32_t acceptorId) : _acceptorId(acceptorId), _scheduel(scheduel)
{
    ::co_create(&_co, nullptr, &runAcceptor, this);
    ::co_resume(_co);
}

int Acceptor::run()
{
    DRY_LOG_TRACE("Acceptor #%u.%u is running", _scheduel->ScheduleId(), _acceptorId);

    auto listenSocket = ::dry::net::Socket::CreateTcpStream();
    DRY_RETURN_IF(listenSocket.Invalid(), dry::error::EC_BAD_LUCK);

    DRY_CALL_LOG(listenSocket.ReuseAddr);
    DRY_CALL_LOG(listenSocket.ReusePort);

    // TODO:
    ::dry::net::InetAddress addr("127.0.0.1", _scheduel->Options().port);
    DRY_CALL_LOG(listenSocket.Bind, addr);

    DRY_CALL_LOG(listenSocket.Listen);
    DRY_CALL_LOG(listenSocket.SetFlags, O_NONBLOCK);

    DRY_LOG_TRACE("Listen socket create succ: %d, listen on port %u",
                  listenSocket.Fd(),
                  static_cast<uint32_t>(_scheduel->Options().port));

    while (true)
    {
        // TODO: check has occupy workers
        int connFd = -1;
        {
            struct sockaddr_in peerAddr = {
                0,
            };
            socklen_t peerAddrLen = sizeof(peerAddr);
            connFd                = ::accept(listenSocket.Fd(), (struct sockaddr*)&peerAddr, &peerAddrLen);
            if (connFd < 0)
            {
                // DRY_LOG_TRACE("Accept socket failed: %d", errno);

                struct pollfd pf = {0};
                pf.fd            = listenSocket.Fd();
                pf.events        = (POLLIN | POLLERR | POLLHUP);
                co_poll(co_get_epoll_ct(), &pf, 1, 1000);
                // TODO: wait until read avaiable
                continue;
            }
        }

        {
            DRY_LOG_TRACE("Client connected: %d", connFd);
            _scheduel->HandleNewConnection(::dry::net::Socket(connFd));
        }
    }

    return 0;
}

}  // end namespace svr
}  // namespace dry
