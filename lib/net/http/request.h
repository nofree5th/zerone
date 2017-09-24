#pragma once
#include "lib/net/http/header.h"
#include "lib/net/url/url.h"
#include "third_party/http-parser/upstream/http_parser.h"

namespace dry
{
namespace net
{
namespace http
{
// A Request represents an HTTP request received by a server
// to be sent by a client.
class Request
{
public:
    Request();

    size_t TryParse(const char* data, const size_t len, bool* completed);
    const ::dry::net::url::Url& Url() const { return _url; }
    const std::string& Body() const { return _body; }
    const ::dry::net::http::Header& Header() const { return _header; }
    const http_parser& Parser() const { return _parser; }
    const std::string ToString() const;

private:
    static int onUrl(http_parser*, const char* at, size_t length);
    static int onHeaderField(http_parser*, const char* at, size_t length);
    static int onHeaderValue(http_parser*, const char* at, size_t length);
    static int onBody(http_parser*, const char* at, size_t length);
    static int onMessageComplete(http_parser*);

private:
    http_parser _parser;
    http_parser_settings _settings;
    bool _completed = false;
    std::string _lastHeaderKey;

    // parse result
    ::dry::net::url::Url _url;
    ::dry::net::http::Header _header;
    std::string _body;
};

}  // end namespace http
}  // end namespace net
}  // end namespace dry
