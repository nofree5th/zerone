#include "lib/svr/tcp_server.h"

#include "lib/svr/server_thread.h"

#include <functional>
#include <memory>
#include <vector>

namespace dry
{
namespace svr
{
void TcpServer::Start()
{
    ServerOptions options = {
        .port           = 12345,  //
        .threadCount    = 1,
        .coroutineCount = 1,
    };

    std::vector<std::unique_ptr<ServerThread>> threads;

    for (uint32_t i = 0; i < options.threadCount; ++i)
    {
        threads.push_back(std::unique_ptr<ServerThread>(new ServerThread(i + 1, options)));
    }

    for (auto& thread : threads)
    {
        thread->Join();
    }
}

}  // end namespace svr
}  // namespace dry
