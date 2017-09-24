#include "lib/svr/server_thread.h"

#include "co_routine.h"

namespace dry
{
namespace svr
{
ServerThread::ServerThread(const uint32_t id, const ServerOptions& options)
    : _id(id), _options(options), _thread(&runServerThread, this)
{
}

void ServerThread::HandleNewConnection(::dry::net::Socket&& newConnection)
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

void ServerThread::AddOccupyWorker(Worker* self)
{
    _occupyWorkers.push_back(self);
}

void ServerThread::runServerThread(ServerThread* pThis)
{
    DRY_LOG_TRACE("ServerThread #%u is runnning", pThis->_id);

    const int ret = pThis->run();

    DRY_LOG_TRACE("ServerThread #%u is run end: %d", pThis->_id, ret);
}

int ServerThread::run()
{
    // init & run workers
    _workers.reserve(_options.CoroutineCount());
    _occupyWorkers.reserve(_options.CoroutineCount());
    for (uint32_t i = 0; i < _options.CoroutineCount(); ++i)
    {
        _workers.emplace_back(std::unique_ptr<Worker>(new Worker(this, i + 1)));
    }

    // init & run acceptor
    _acceptor.reset(new Acceptor(this, ScheduleId()));

    // run forever
    co_eventloop(co_get_epoll_ct(), 0, 0);

    return 0;
}

ServerThread::~ServerThread()
{
    Join();
}

}  // end namespace svr
}  // namespace dry
