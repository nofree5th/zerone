#include "lib/net/http/response.h"

#include <string>
#include "lib/log/logger.h"

constexpr auto MAX_STATUS_NUMBER = 600;  // real 5xx

const char** getStatusMessageMap()
{
    // Should only call once
    const auto initStatusMessageMap = []() {
        static const char* g_statusMessageMap[MAX_STATUS_NUMBER + 1] = {
            0,
        };
// Generate status message map
// Like: const std::string HTTP_STATUS_200
// static_assert(num < 0 || num > MAX_STATUS_NUMBER, "Invalid status num");
#define DRY_XX(num, name, str) g_statusMessageMap[num] = #str;

        HTTP_STATUS_MAP(DRY_XX)
#undef DRY_XX
        return g_statusMessageMap;
    };

    static const char** s_map = initStatusMessageMap();

    return s_map;
}

static const char* getStatusMessage(const uint32_t status)
{
    if (status > MAX_STATUS_NUMBER) return "";
    return getStatusMessageMap()[status];
}

namespace dry
{
namespace net
{
namespace http
{
Response::Response(const int majorVersion, const int minorVersion)
    : _majorVersion(majorVersion), _minorVersion(minorVersion)
{
}

void Response::SetStatus(const int status, const std::string& msg)
{
    _status    = status;
    _statusMsg = msg;
}

const std::string Response::ToString()
{
    std::string result;
    // append Response line
    {
        result.append("HTTP/");
        result.append(std::to_string(_majorVersion));
        result.append(".");
        result.append(std::to_string(_minorVersion));

        result.append(" ");

        result.append(std::to_string(_status));
        result.append(" ");

        if (!_statusMsg.empty())
            result.append(_statusMsg);
        else
            result.append(getStatusMessage(_status));
        result.append("\r\n");
    }
    // process headers
    if (!_body.empty())
    {
        _header.Set("Content-Length", std::to_string(_body.size()));
    }

    // append headers
    if (!_header.Empty())
    {
        result.append(_header.ToString());
    }

    result.append("\r\n");
    // if POST append body
    if (!_body.empty())
    {
        result.append(_body);
    }

    return result;
}

}  // end namespace http
}  // end namespace net
}  // end namespace dry
