#include "comm/comm.h"
#include "gflags/gflags.h"

namespace dry
{
namespace comm
{
void Init(int* argc, char*** argv)
{
    gflags::ParseCommandLineFlags(argc, argv, false);
}
}  // end namespace comm
}  // end namespace dry
