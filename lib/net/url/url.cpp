#include "lib/net/url/url.h"

#include "third_party/http-parser/upstream/http_parser.h"

namespace dry
{
namespace net
{
namespace url
{
int Url::ParseFromArray(const char* rawUrl, const size_t rawUrlLen, const bool isConnect) noexcept
{
    http_parser_url urlParser;
    http_parser_url_init(&urlParser);
    const int parseRet = http_parser_parse_url(rawUrl, rawUrlLen, isConnect ? 1 : 0, &urlParser);
    if (parseRet != 0) return parseRet;

    for (int i = 0; i < UF_MAX; i++)
    {
        if ((urlParser.field_set & (1 << i)) == 0)
        {
            // field not set
            continue;
        }
        const auto& field = urlParser.field_data[i];
        switch (i)
        {
            case UF_SCHEMA:
                this->schema.assign(&rawUrl[field.off], field.len);
                break;
            case UF_HOST:
                this->host.assign(&rawUrl[field.off], field.len);
                break;
            case UF_PORT:
                this->port = urlParser.port;
                break;
            case UF_PATH:
                this->path.assign(&rawUrl[field.off], field.len);
                break;
            case UF_QUERY:
                this->rawQuery.assign(&rawUrl[field.off], field.len);
                break;
            case UF_FRAGMENT:
                this->fragment.assign(&rawUrl[field.off], field.len);
                break;
            default:
                // ignored
                ;
        }
    }

    return 0;
}

}  // end namespace url
}  // end namespace net
}  // end namespace dry
