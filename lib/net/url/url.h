#pragma once
#include <cinttypes>
#include <string>

namespace dry
{
namespace net
{
namespace url
{
struct Url
{
    std::string schema;  // eg. "http" / "ftp"
    std::string host;    // host or host:port
    uint16_t port = 0;   // 0 for default 80
    std::string path;
    std::string rawQuery;  // encoded query values, without '?'
    std::string fragment;  // fragment for references, without '#'

    int ParseFromArray(const char* rawUrl, const size_t rawUrlLen, const bool isConnect = false) noexcept;
    int ParseFromString(const std::string& rawUrl, const bool isConnect = false) noexcept
    {
        return ParseFromArray(rawUrl.data(), rawUrl.size(), isConnect);
    }
};

}  // end namespace url
}  // end namespace net
}  // end namespace dry
