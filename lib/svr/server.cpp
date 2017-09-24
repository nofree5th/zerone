#include "lib/svr/server.h"

#include "lib/svr/server_thread.h"

#include <functional>
#include <memory>
#include <vector>

namespace dry
{
namespace svr
{
int Server::StartUntilWorldOfEnd()
{
    std::vector<std::unique_ptr<ServerThread>> threads;

    for (uint32_t i = 0; i < _options.ThreadCount(); ++i)
    {
        threads.push_back(std::unique_ptr<ServerThread>(new ServerThread(i + 1, _options)));
    }

    for (auto& thread : threads)
    {
        thread->Join();
    }
    return 0;
}

}  // end namespace svr
}  // namespace dry
