#include "lib/core.h"
#include "lib/svr/server.h"

int main(int argc, char* argv[])
{
    dry::lib::Init(&argc, &argv);
    dry::log::Instance()->Init("", dry::log::LOG_LEVEL_TRACE);

    dry::svr::Server svr;
    svr.Start();
}
