#pragma once

#include <cinttypes>
#include <string>

#include "lib/error/error.h"
#include "lib/log/logger.h"

namespace dry
{
namespace lib
{
void Init(int* argc, char*** argv);
}  // end namespace lib
}  // end namespace dry
