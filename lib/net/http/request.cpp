#include "lib/net/http/request.h"
#include <cassert>

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
    // TODO:
    return std::string() + "Url:" + _url.ToString() + ", Body:" + _body;
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

size_t Request::TryParse(const char* data, size_t len, bool* completed)
{
    assert(completed != nullptr);
    assert(!_completed);

    const size_t parsedBytes = http_parser_execute(&_parser, &_settings, data, len);
    *completed               = _completed;
    return parsedBytes;
}

}  // end namespace http
}  // end namespace net
}  // end namespace dry
