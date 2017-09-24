#pragma once
#include <memory>
#include <thread>
#include <vector>

#include "boost/core/noncopyable.hpp"
#include "lib/error/error.h"
#include "lib/log/logger.h"

#include "lib/svr/acceptor.h"
#include "lib/svr/worker.h"

namespace dry
{
namespace svr
{
class Worker;
class Acceptor;
class ServerThread : boost::noncopyable
{
public:
    ServerThread(const uint32_t id, const ServerOptions& options);
    ~ServerThread();

    const uint32_t ScheduleId() const { return _id; }
    void Join() { _thread.join(); }
    const ServerOptions& Options() const { return _options; }

protected:
    friend class Worker;
    // Worker call this
    void AddOccupyWorker(Worker* self);

    friend class Acceptor;
    // Acceptor call this
    void HandleNewConnection(::dry::net::Socket&& newConnection);

private:
    int run();
    static void runServerThread(ServerThread* pThis);

private:
    uint32_t _id;
    ServerOptions _options;
    std::thread _thread;

    std::vector<std::unique_ptr<Worker>> _workers;
    std::vector<Worker*> _occupyWorkers;  // don't own Worker
    std::unique_ptr<Acceptor> _acceptor;
};

}  // end namespace svr
}  // namespace dry
