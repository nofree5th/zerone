#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <memory>
#include <thread>
#include <vector>
#include "comm/comm.h"

static void writeLog(int id)
{
    DRY_LOG_TRACE("#%d hello trace log: %d", id, 123);
    DRY_LOG_DEBUG("#%d hello debug log: %d", id, 123);
    DRY_LOG_WARN("#%d hello warn log: %d", id, 123);
    DRY_LOG_INFO("#%d hello info log: %d", id, 123);
    DRY_LOG_ERROR("#%d hello error log: %d", id, 123);
}

int main(int argc, char* argv[])
{
    dry::comm::Init(&argc, &argv);

    dry::log::Instance()->Init("", dry::log::LOG_LEVEL_TRACE);

    std::vector<std::unique_ptr<std::thread>> threads;

    for (int i = 0; i < 10; ++i)
    {
        threads.emplace_back(std::unique_ptr<std::thread>(new std::thread(writeLog, i)));
    }

    for (auto& t : threads)
    {
        t->join();
    }

    printf("Enter any key to break(%u, %u).\n", (uint32_t)getpid(), (uint32_t)syscall(SYS_gettid));
    scanf("%*c");
    printf("End\n");
}
