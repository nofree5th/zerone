#pragma once
#include "lib/net/http/header.h"
#include "third_party/http-parser/upstream/http_parser.h"

namespace dry
{
namespace net
{
namespace http
{
// A Response represents an HTTP response received from a client
// to be sent by a server.
class Response
{
public:
    Response(const int majorVersion = 1, const int minorVersion = 1);

    ::dry::net::http::Header* MutableHeader() { return &_header; }
    // status defined in http_parser.h like HTTP_STATUS_OK / HTTP_STATUS_NOT_FOUND / HTTP_STATUS_INTERNAL_SERVER_ERROR
    void SetStatus(const int status, const std::string& msg = "");
    const std::string ToString();
    void SetBody(const std::string& body) { _body = body; }

private:
    int _majorVersion;
    int _minorVersion;
    ::dry::net::http::Header _header;
    std::string _body;
    int _status;
    std::string _statusMsg;
};

}  // end namespace http
}  // end namespace net
}  // end namespace dry
