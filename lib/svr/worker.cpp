#include "lib/svr/worker.h"

#include "co_routine.h"
#include "lib/svr/server_thread.h"
namespace dry
{
namespace svr
{
void* Worker::runWorker(void* pThis)
{
    ::co_enable_hook_sys();
    reinterpret_cast<Worker*>(pThis)->run();
    return nullptr;
}

Worker::Worker(ServerThread* scheduel, const uint32_t workerId)
    : _scheduel(scheduel), _workerId(workerId), _connectionHandler(scheduel->Options())
{
    ::co_create(&_co, nullptr, &runWorker, this);
    ::co_resume(_co);
}

void Worker::HandleNewConnection(::dry::net::Socket&& newConnection)
{
    // NOTE: check _co not running
    _connectionHandler.AttachNewConnection(std::move(newConnection));
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
}  // end namespace svr
}  // namespace dry
