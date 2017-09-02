#include "lib/log/logger.h"

#include <fcntl.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <atomic>
#include <cinttypes>
#include <iomanip>
#include <mutex>
#include <sstream>

#include "lib/error/error.h"

namespace dry
{
namespace log
{
class LoggerImpl
{
public:
    LoggerImpl() : _level(LOG_LEVEL_TRACE), _fd(-1) {}
    ~LoggerImpl()
    {
        std::lock_guard<std::mutex> lck(_mutex);
        release();
    }

    int Init(const std::string& filePath, const LogLevel level)
    {
        DRY_RETURN_IF(filePath.empty(), ::dry::error::EC_INVALID_PARAMS);

        // permission 0666
        const int fd = open(filePath.c_str(),
                            O_CREAT | O_APPEND | O_WRONLY | O_NOCTTY | O_NONBLOCK | O_LARGEFILE,
                            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
        DRY_RETURN_IF_F(fd == -1, ::dry::error::EC_FILE, "Open file: %s failed, errno: %d", filePath.c_str(), errno);

        {
            std::lock_guard<std::mutex> lck(_mutex);

            release();

            _fd       = fd;
            _filePath = filePath;
            _level    = level;
        }

        return 0;
    }
    inline const bool NeedLog(const LogLevel curLevel) const { return curLevel <= _level; }
    int Log(const char* format, va_list va)
    {
        constexpr size_t BUF_SIZE = 2048;
        char buffer[BUF_SIZE];
        ssize_t count = vsnprintf(buffer, sizeof(buffer), format, va);
        if (count < 0)
        {
            return ::dry::error::EC_MEMORY;
        }

        if (static_cast<size_t>(count) >= sizeof(buffer))
        {
            static_assert(sizeof(buffer) >= 4, "buffer size should >= 4");
            buffer[sizeof(buffer) - 4] = '.';
            buffer[sizeof(buffer) - 3] = '.';
            buffer[sizeof(buffer) - 2] = '.';
        }

        {
            std::lock_guard<std::mutex> lck(_mutex);
            if (_fd != -1)
            {
                (void)TEMP_FAILURE_RETRY(write(_fd, buffer, count));
            }
            else
            {
                fputs(buffer, stderr);
            }
        }

        return 0;
    }

private:
private:
    void release()
    {
        if (_fd != -1)
        {
            close(_fd);
            _fd = -1;
        }
        _filePath.clear();
    }

private:
    std::string _filePath;
    std::atomic<uint32_t> _level;
    int _fd;
    std::mutex _mutex;
};

// === public ===
Logger* Instance()
{
    static Logger s_inst;

    return &s_inst;
}

Logger::Logger() : _impl(new LoggerImpl())
{
}

int Logger::Printf(const char* format, ...)
{
    va_list va;
    va_start(va, format);
    const int ret = _impl->Log(format, va);
    va_end(va);
    return ret;
}

int Logger::Init(const std::string& filePath, const LogLevel level)
{
    return _impl->Init(filePath, level);
}

const bool Logger::NeedLog(const LogLevel curLevel) const
{
    return _impl->NeedLog(curLevel);
}

std::string Logger::CalcHeader(const char* levelDesc)
{
    struct tm tm;
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    localtime_r(&tv.tv_sec, &tm);

    const auto ms = tv.tv_usec / 1000;

    std::ostringstream s;
    s << levelDesc << " "                        // level
      << std::setfill('0')                       // fill 0
      << std::setw(2) << tm.tm_mon + 1 << '-'    // month
      << std::setw(2) << tm.tm_mday << ' '       // day
      << std::setw(2) << tm.tm_hour << ':'       // hour
      << std::setw(2) << tm.tm_min << ':'        // min
      << std::setw(2) << tm.tm_sec << '.'        // sec
      << std::setw(3) << ms << ' '               // ms
      << getpid() << ',' << syscall(SYS_gettid)  // pid,tid
        ;
    return s.str();
}

}  // end namespace log
}  // end namespace dry
