#pragma once

namespace dry
{
namespace error
{
enum ErrorCode
{
    // 1xx comm
    EC_FILE   = -10000001,
    EC_MEMORY = -10000002,

    // 5xx server
    EC_TODO       = -10000500,
    EC_IMPOSSIBLE = -10000501,
    EC_BAD_LUCK   = -10000502,
    EC_CONF       = -10000503,

    // 4xx client
    EC_INVALID_PARAMS = -10000400,
    EC_NO_PRIVILEGES  = -10000401,
    EC_NOT_EXIST      = -10000404,
};

}  // end namespace error
}  // end namespace dry
