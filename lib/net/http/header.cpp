#include "lib/net/http/header.h"

namespace dry
{
namespace net
{
namespace http
{
const static std::string g_empty;

void Header::Add(const std::string& key, const std::string& value) noexcept
{
    _headers[key].push_back(value);
}

void Header::Set(const std::string& key, const std::string& value) noexcept
{
    auto& values = _headers[key];
    values.clear();

    values.push_back(value);
}
void Header::Set(const std::string& key, std::string&& value) noexcept
{
    auto& values = _headers[key];
    values.clear();

    values.emplace_back(std::move(value));
}

const std::string& Header::Get(const std::string& key) noexcept
{
    auto itr = _headers.find(key);
    if (itr == _headers.end()) return g_empty;

    auto& values = itr->second;
    if (values.empty()) return g_empty;

    return values[0];  // first value
}

}  // end namespace http
}  // end namespace net
}  // end namespace dry
