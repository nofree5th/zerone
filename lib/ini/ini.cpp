#include "lib/ini/ini.h"

#include <fstream>

#include "boost/algorithm/string.hpp"
#include "boost/smart_ptr.hpp"
#include "lib/error/error.h"
#include "lib/log/logger.h"

namespace dry
{
namespace ini
{
const static std::string EMPTY_STR;
const std::string& IniParser::GetValue(const std::string& section, const std::string& key)
{
    const auto& sectionItr = _sectionMap.find(section);
    if (sectionItr == _sectionMap.end()) return EMPTY_STR;
    const auto& curSectionMap = sectionItr->second;

    const auto valueItr = curSectionMap.find(key);
    if (valueItr == curSectionMap.end()) return EMPTY_STR;
    return valueItr->second;
}

static int readAllFileContent(const std::string& filePath, std::string* fileContent)
{
    std::ifstream is(filePath, std::ifstream::binary);
    DRY_RETURN_IF_F(!is, dry::error::EC_FILE, "File %s not exist", filePath.c_str());

    is.seekg(0, is.end);
    const int length = is.tellg();
    if (length == 0)
    {
        fileContent->clear();
        return 0;
    }

    DRY_LOG_TRACE("File size: %d", length);

    boost::scoped_array<char> buffer(new char[length]);

    // read data as a block:
    is.seekg(0, is.beg);
    is.read(buffer.get(), length);

    DRY_RETURN_IF_F(!is, dry::error::EC_FILE, "Read file %s failed", filePath.c_str());

    fileContent->assign(buffer.get(), length);

    return 0;
}

int IniParser::InitFromFile(const std::string& filePath)
{
    std::string fileContent;
    DRY_CALL_LOG(readAllFileContent, filePath, &fileContent);

    return InitFromBuffer(fileContent);
}

int IniParser::InitFromBuffer(const std::string& buffer)
{
    size_t offset = 0;
    size_t lineNo = 0;
    std::string curSection;
    while (offset < buffer.size())
    {
        ++lineNo;

        // get line
        const size_t start = offset;
        // moveToNextLine
        while (offset < buffer.size())
        {
            const char cur = buffer[offset++];
            if (cur == '\n' || cur == '\r')
            {
                break;
            }
        }
        std::string line(buffer, start, offset - start);

        boost::trim(line);
        if (line.empty() || line[0] == '#' || line[0] == ';') continue;

        // process line
        if (line[0] == '[')
        {
            if (line.back() != ']')
            {
                DRY_LOG_WARN("Unclosed section , line %zu : {%s}", lineNo, line.c_str());
                continue;
            }

            curSection = boost::trim_copy(line.substr(1 /*skip [*/, line.size() - 2 /*exclude []*/));
            continue;
        }
        else
        {
            if (curSection.empty())
            {
                DRY_LOG_WARN("No section before key-value item, line %zu : {%s}", lineNo, line.c_str());
            }
        }

        const auto pos = line.find('=');
        if (pos == line.npos)
        {
            DRY_LOG_WARN("Invalid key-value item: no '=' found, line %zu : {%s}", lineNo, line.c_str());
            continue;
        }

        if (pos == 0)
        {
            DRY_LOG_WARN("Invalid key-value item: missing key, line %zu : {%s}", lineNo, line.c_str());
            continue;
        }

        const auto& curKey   = boost::trim_copy(line.substr(0, pos - 1));
        const auto& curValue = boost::trim_copy(line.substr(pos + 1));

        if (curKey.empty())
        {
            DRY_LOG_WARN("Empty key not valid, section: %s, line %zu : {%s}", curSection.c_str(), lineNo, line.c_str());
            continue;
        }

        DRY_LOG_TRACE("Line %zu: [%s] {%s} = {%s}", lineNo, curSection.c_str(), curKey.c_str(), curValue.c_str());
        _sectionMap[curSection][curKey] = std::move(curValue);
    }

    return 0;
}

}  // end namespace ini
}  // end namespace dry
