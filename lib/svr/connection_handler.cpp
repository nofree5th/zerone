#include "lib/svr/connection_handler.h"
#include "lib/log/logger.h"
#include "lib/net/http/request.h"

namespace dry
{
namespace svr
{
#if 0
static void echoProcessing(::dry::net::Socket& socket)
{
    // RecvingRequest
    constexpr auto BUF_SIZE = 1024;
    char buf[BUF_SIZE + 1];
    const ssize_t ret = read(socket.Fd(), buf, BUF_SIZE);
    if (ret == 0)
    {
        DRY_LOG_INFO("Peer close connection: %d", socket.Fd());
        socket.Release();
        return;
    }
    if (ret == -1)
    {
        DRY_LOG_ERROR("Read error: %d, errno: %d", socket.Fd(), errno);
        socket.Release();
        return;
    }
    buf[ret + 1] = '\0';
    DRY_LOG_INFO("Read req succ(%zu): %s", static_cast<size_t>(ret), buf);

    // Processing
    // SendingResponse
    {
        const ssize_t writeRet = write(socket.Fd(), buf, ret);
        if (writeRet == -1)
        {
            DRY_LOG_ERROR("Write error: %d, errno: %d", socket.Fd(), errno);
            socket.Release();
            return;
        }
        if (writeRet != ret)
        {
            DRY_LOG_ERROR("Write partial error: %d, size %zu -> %zu",
                          socket.Fd(),
                          static_cast<size_t>(ret),
                          static_cast<size_t>(writeRet));
            socket.Release();
            return;
        }
        DRY_LOG_TRACE("Send all success, don't keep alive");
    }
}
#endif

static void httpProcessing(::dry::net::Socket& socket, ::dry::bytes::Buffer& buf)
{
__KEEP_ALIVE:
    buf.Reset();
    // RecvingRequest
    dry::net::http::Request req;
    while (true)
    {
        // read
        {
            constexpr auto BUF_SIZE = 1024;
            const ssize_t ret       = read(socket.Fd(), buf.WritePtr(BUF_SIZE), BUF_SIZE);
            if (ret == 0)
            {
                DRY_LOG_INFO("Peer close connection: %d", socket.Fd());
                return;
            }
            if (ret == -1)
            {
                DRY_LOG_ERROR("Read error: %d, errno: %d", socket.Fd(), errno);
                return;
            }
            buf.Write(ret);

            DRY_LOG_TRACE("Read %d bytes", static_cast<int>(ret));
        }

        // try parse
        {
            size_t dataLen   = 0;
            const char* data = buf.ReadPtr(&dataLen);
            DRY_LOG_TRACE("TryParse %zu bytes: %s", dataLen, std::string(data, dataLen).c_str());

            bool isCompleted   = false;
            size_t parsedBytes = 0;
            const int parseRet = req.TryParse(data, dataLen, &parsedBytes, &isCompleted);
            if (parseRet != 0) return;  // parse meet error

            if (isCompleted)
            {
                buf.Reset();
                break;
            }
            buf.Read(parsedBytes);

            DRY_LOG_INFO("Wait req to complete: %zu bytes got", parsedBytes);
        }
    }
    DRY_LOG_INFO("Read req succ: \n%s", req.ToString().c_str());

    // Processing
    // SendingResponse
    {
        std::string response   = R"__(HTTP/1.1 200 OK

hello
)__";
        const ssize_t writeRet = write(socket.Fd(), response.data(), response.size());
        if (writeRet == -1)
        {
            DRY_LOG_ERROR("Write error: %d, errno: %d", socket.Fd(), errno);
            return;
        }
        if ((size_t)writeRet != response.size())
        {
            DRY_LOG_ERROR("Write partial error: %d, size %zu -> %zu",
                          socket.Fd(),
                          static_cast<size_t>(response.size()),
                          static_cast<size_t>(writeRet));
            return;
        }
        if (req.KeepAlive())
        {
            goto __KEEP_ALIVE;
        }
        DRY_LOG_TRACE("Send all success, don't keep alive");
    }
}

void ConnectionHandler::AttachNewConnection(::dry::net::Socket&& newConnection)
{
    _socket = std::move(newConnection);
}

void ConnectionHandler::Processing()
{
    DRY_LOG_INFO("NewConnection: %d", _socket.Fd());

    // RecvingRequest
    // Processing
    // SendingResponse
    // echoProcessing(_socket);
    httpProcessing(_socket, _buf);

    _socket.Release();
}

}  // end namespace svr
}  // namespace dry
