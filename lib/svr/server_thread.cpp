#include "lib/svr/server_thread.h"

#include <fcntl.h>
#include <sys/socket.h>
#include <thread>
#include <vector>

#include "co_routine.h"
#include "lib/net/socket.h"

// libco
int co_accept(int fd, struct sockaddr* addr, socklen_t* len);

namespace dry
{
namespace svr
{
class ServerThreadImpl;

class ConnectionHandler : boost::noncopyable
{
public:
    // options should live long then ConnectionHandler
    explicit ConnectionHandler(const ServerOptions& options) : _options(options) {}
    void AttachNewConnection(::dry::net::Socket&& newConnection) { _socket = std::move(newConnection); }
    bool HasConnection() const { return _socket.Valid(); }
    void Processing()
    {
        DRY_LOG_INFO("NewConnection: %d", _socket.Fd());

        // RecvingRequest
        // Processing
        // SendingResponse
        echoProcessing(_socket);

        _socket.Release();
    }

private:
    static void echoProcessing(::dry::net::Socket& socket)
    {
        // RecvingRequest
        constexpr auto BUF_SIZE = 1024;
        char buf[BUF_SIZE + 1];
        const ssize_t ret = read(socket.Fd(), buf, BUF_SIZE);
        if (ret == 0)
        {
            DRY_LOG_INFO("Peer close connection: %d", socket.Fd());
            socket.Release();
            return;
        }
        if (ret == -1)
        {
            DRY_LOG_ERROR("Read error: %d, errno: %d", socket.Fd(), errno);
            socket.Release();
            return;
        }
        buf[ret + 1] = '\0';
        DRY_LOG_INFO("Read req succ(%zu): %s", static_cast<size_t>(ret), buf);

        // Processing
        // SendingResponse
        {
            const ssize_t writeRet = write(socket.Fd(), buf, ret);
            if (writeRet == -1)
            {
                DRY_LOG_ERROR("Write error: %d, errno: %d", socket.Fd(), errno);
                socket.Release();
                return;
            }
            if (writeRet != ret)
            {
                DRY_LOG_ERROR("Write partial error: %d, size %zu -> %zu",
                              socket.Fd(),
                              static_cast<size_t>(ret),
                              static_cast<size_t>(writeRet));
                socket.Release();
                return;
            }
            DRY_LOG_TRACE("Send all success, don't keep alive");
        }
    }

private:
    ::dry::net::Socket _socket;
    const ServerOptions& _options;
};

class Acceptor : boost::noncopyable
{
public:
    Acceptor(ServerThreadImpl* scheduel, const uint32_t acceptorId) : _acceptorId(acceptorId), _scheduel(scheduel)
    {
        ::co_create(&_co, nullptr, &runAcceptor, this);
        ::co_resume(_co);
    }
    static void* runAcceptor(void* pThis)
    {
        ::co_enable_hook_sys();
        (void)reinterpret_cast<Acceptor*>(pThis)->run();
        // ret ignored
        return nullptr;
    }
    int run();

private:
    uint32_t _acceptorId;
    ServerThreadImpl* _scheduel;
    stCoRoutine_t* _co;
};

class Worker : boost::noncopyable
{
public:
    Worker(ServerThreadImpl* scheduel, const uint32_t workerId);

    static void* runWorker(void* pThis)
    {
        ::co_enable_hook_sys();
        reinterpret_cast<Worker*>(pThis)->run();
        return nullptr;
    }

    void run();
    void HandleNewConnection(::dry::net::Socket&& newConnection)
    {
        // NOTE: check _co not running
        _connectionHandler.AttachNewConnection(std::move(newConnection));
        ::co_resume(_co);
    }

private:
    ServerThreadImpl* _scheduel;
    uint32_t _workerId;
    stCoRoutine_t* _co;
    ConnectionHandler _connectionHandler;
};

class ServerThreadImpl : boost::noncopyable
{
public:
    ServerThreadImpl(const uint32_t id, const ServerOptions& options)
        : _id(id), _thread(&runServerThread, this), _options(options)
    {
    }
    const uint32_t ScheduleId() const { return _id; }
    void HandleNewConnection(::dry::net::Socket&& newConnection);
    void AddOccupyWorker(Worker* self);
    void Join() { _thread.join(); }
    const ServerOptions& Options() const { return _options; }

private:
    static void runServerThread(ServerThreadImpl* pThis)
    {
        DRY_LOG_TRACE("ServerThread #%u is runnning", pThis->_id);

        const int ret = pThis->run();

        DRY_LOG_TRACE("ServerThread #%u is run end: %d", pThis->_id, ret);
    }
    int run();

private:
    uint32_t _id;
    std::thread _thread;
    ServerOptions _options;

    std::vector<std::unique_ptr<Worker>> _workers;
    std::vector<Worker*> _occupyWorkers;  // don't own Worker
    std::unique_ptr<Acceptor> _acceptor;
};

Worker::Worker(ServerThreadImpl* scheduel, const uint32_t workerId)
    : _scheduel(scheduel), _workerId(workerId), _connectionHandler(scheduel->Options())
{
    ::co_create(&_co, nullptr, &runWorker, this);
    ::co_resume(_co);
}

void Worker::run()
{
    DRY_LOG_TRACE("Worker #%u.%u is running", _scheduel->ScheduleId(), _workerId);

    for (;;)
    {
        if (_connectionHandler.HasConnection())
        {
            _connectionHandler.Processing();
        }
        _scheduel->AddOccupyWorker(this);

        ::co_yield_ct();
    }
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

void ServerThreadImpl::HandleNewConnection(::dry::net::Socket&& newConnection)
{
    if (_occupyWorkers.empty())
    {
        DRY_LOG_WARN("No occupy workers to handle new connection: %d", newConnection.Fd());
        return;
    }
    auto* worker = _occupyWorkers.back();
    _occupyWorkers.pop_back();

    worker->HandleNewConnection(std::move(newConnection));
}

void ServerThreadImpl::AddOccupyWorker(Worker* self)
{
    _occupyWorkers.push_back(self);
}

int ServerThreadImpl::run()
{
    // init & run workers
    _workers.reserve(_options.coroutineCount);
    _occupyWorkers.reserve(_options.coroutineCount);
    if (_options.coroutineCount != 1)
    {
        DRY_LOG_ERROR("Co count only 1");
        return __LINE__;
    }
    for (uint32_t i = 0; i < _options.coroutineCount; ++i)
    {
        _workers.emplace_back(std::unique_ptr<Worker>(new Worker(this, i + 1)));
    }

    // init & run acceptor
    _acceptor.reset(new Acceptor(this, ScheduleId()));

    // run forever
    co_eventloop(co_get_epoll_ct(), 0, 0);

    return 0;
}

ServerThread::ServerThread(const uint32_t id, const ServerOptions& options) : _impl(new ServerThreadImpl(id, options))
{
}

ServerThread::~ServerThread()
{
    delete _impl;
    _impl = nullptr;
}

void ServerThread::Join()
{
    return _impl->Join();
}

}  // end namespace svr
}  // namespace dry
