#include "lib/net/http/request.h"
#include <cassert>
#include "lib/log/logger.h"

namespace dry
{
namespace net
{
namespace http
{
Request::Request()
{
    http_parser_init(&_parser, HTTP_REQUEST);
    _parser.data = this;

    http_parser_settings_init(&_settings);
    _settings.on_url              = onUrl;
    _settings.on_header_field     = onHeaderField;
    _settings.on_header_value     = onHeaderValue;
    _settings.on_body             = onBody;
    _settings.on_message_complete = onMessageComplete;
}

const std::string Request::ToString() const
{
    std::string result;
    // append Request line
    {
        result.append(http_method_str(static_cast<http_method>(_parser.method)));

        result.append(" ");
        result.append(_url.ToString());
        result.append(" ");

        result.append("HTTP/");
        result.append(std::to_string(_parser.http_major));
        result.append(".");
        result.append(std::to_string(_parser.http_minor));
        result.append("\r\n");
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
        result.append("\r\n");
    }

    return result;
}

int Request::onUrl(http_parser* parser, const char* at, size_t length)
{
    Request* pThis = reinterpret_cast<Request*>(parser->data);
    return pThis->_url.ParseFromArray(at, length);
}

int Request::onHeaderField(http_parser* parser, const char* at, size_t length)
{
    Request* pThis = reinterpret_cast<Request*>(parser->data);

    pThis->_lastHeaderKey.assign(at, length);

    return 0;
}

int Request::onHeaderValue(http_parser* parser, const char* at, size_t length)
{
    Request* pThis = reinterpret_cast<Request*>(parser->data);

    pThis->_header.Set(pThis->_lastHeaderKey, std::string(at, length));
    pThis->_lastHeaderKey.clear();

    return 0;
}

int Request::onBody(http_parser* parser, const char* at, size_t length)
{
    Request* pThis = reinterpret_cast<Request*>(parser->data);
    pThis->_body.assign(at, length);
    return 0;
}

int Request::onMessageComplete(http_parser* parser)
{
    Request* pThis = reinterpret_cast<Request*>(parser->data);

    pThis->_completed = true;
    return 0;
}

int Request::TryParse(const char* data, size_t len, size_t* parsedBytes, bool* completed)
{
    assert(completed != nullptr);
    assert(!_completed);

    *parsedBytes = http_parser_execute(&_parser, &_settings, data, len);
    *completed   = _completed;
    if (_parser.http_errno != 0)
    {
        DRY_LOG_ERROR("Parser meet error: %s, str(%zu): %s",
                      http_errno_name(HTTP_PARSER_ERRNO(&_parser)),
                      len,
                      std::string(data, std::max(len, size_t(128))).c_str());
        return _parser.http_errno;
    }
    return 0;
}

}  // end namespace http
}  // end namespace net
}  // end namespace dry
