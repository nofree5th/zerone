#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace dry
{
namespace net
{
namespace http
{
class Header
{
public:
    // Like 'Set-Cookie'
    void Add(const std::string& key, const std::string& value) noexcept;
    // Like 'Context-Type'
    void Set(const std::string& key, const std::string& value) noexcept;
    void Set(const std::string& key, std::string&& value) noexcept;
    // Get the first value associated with the given key.
    const std::string& Get(const std::string& key) noexcept;

private:
    std::unordered_map<std::string, std::vector<std::string>> _headers;
};

}  // end namespace http
}  // end namespace net
}  // end namespace dry
