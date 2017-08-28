#include "comm/comm.h"

int main(int argc, char* argv[])
{
    dry::comm::Init(&argc, &argv);

    dry::log::Instance()->Init("", dry::log::LOG_LEVEL_TRACE);

    DRY_LOG_TRACE("hello trace log: %d", 123);
    DRY_LOG_DEBUG("hello debug log: %d", 123);
    DRY_LOG_WARN("hello warn log: %d", 123);
    DRY_LOG_INFO("hello info log: %d", 123);
    DRY_LOG_ERROR("hello error log: %d", 123);
}
