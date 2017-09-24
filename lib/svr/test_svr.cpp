#include "lib/core.h"
#include "lib/svr/server.h"

int main(int argc, char* argv[])
{
    dry::svr::ServerOptions options;
    options.ListenOn(12345);

    const auto threadCount    = 1;
    const auto coroutineCount = 1;
    options.SetConcurrency(threadCount, coroutineCount);

    dry::lib::Init(&argc, &argv);
    dry::log::Instance()->Init("", dry::log::LOG_LEVEL_TRACE);

    dry::svr::Server svr(options);

    return svr.StartUntilWorldOfEnd();
}
