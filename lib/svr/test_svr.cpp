#include "gflags/gflags.h"
#include "lib/core.h"
#include "lib/svr/server.h"

DEFINE_int32(port, 12345, "Port of this server");
DEFINE_int32(threadCount, 1, "Thread count of this server");
DEFINE_int32(coroutineCount, 1, "Coroutine count per server thread count");

int main(int argc, char* argv[])
{
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    dry::svr::ServerOptions options;
    options.ListenOn(FLAGS_port);

    options.SetConcurrency(FLAGS_threadCount, FLAGS_coroutineCount);

    dry::lib::Init(&argc, &argv);
    dry::log::Instance()->Init("", dry::log::LOG_LEVEL_TRACE);

    dry::svr::Server svr(options);

    return svr.StartUntilWorldOfEnd();
}
