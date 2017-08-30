#pragma once

#include <string>
#include <unordered_map>

namespace dry
{
namespace ini
{
class IniParser
{
public:
    int InitFromFile(const std::string& filePath);
    int InitFromBuffer(const std::string& buffer);

    typedef std::unordered_map<std::string, std::string> KeyValueMap;
    typedef std::unordered_map<std::string, KeyValueMap> SectionMap;

    // Not exist section or key will return empty value
    const std::string& GetValue(const std::string& section, const std::string& key);

private:
    SectionMap _sectionMap;
};

}  // end namespace ini
}  // end namespace dry
