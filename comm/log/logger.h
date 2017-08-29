#pragma once

#include <memory>
#include <string>
#include "comm/log/logger_helper.h"

namespace dry
{
namespace log
{
enum ErrorCode
{
    EC_ = -10001001,
};

enum LogLevel
{
    LOG_LEVEL_NONE  = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN  = 2,
    LOG_LEVEL_INFO  = 3,
    LOG_LEVEL_DEBUG = 8,
    LOG_LEVEL_TRACE = 16,
};

class LoggerImpl;
class Logger
{
public:
    Logger();
    int Printf(const char* format, ...) __attribute__((format(printf, 2, 3)));
    int Init(const std::string& filePath, const LogLevel level);
    const bool NeedLog(const LogLevel curLevel) const;
    static std::string CalcHeader(const char* levelDesc);

private:
    std::unique_ptr<LoggerImpl> _impl;
};

Logger* Instance();

}  // end namespace log
}  // end namespace dry

#define __DRY_LOG(LEVEL, LEVEL_DESC, FMT_BEGIN, FMT_END, FMT, ARGS...)                         \
    do                                                                                         \
    {                                                                                          \
        if (::dry::log::Instance()->NeedLog(::dry::log::LOG_LEVEL_##LEVEL))                    \
        {                                                                                      \
            ::dry::log::Instance()->Printf("%s" FMT_BEGIN FMT FMT_END " %s %s +%d\n",          \
                                           ::dry::log::Logger::CalcHeader(LEVEL_DESC).c_str(), \
                                           ##ARGS,                                             \
                                           __func__,                                           \
                                           __FILE__,                                           \
                                           __LINE__);                                          \
        }                                                                                      \
    } while (0)

#define DRY_LOG_TRACE(FMT, ARGS...) __DRY_LOG(TRACE, "T", " \033[0;32m", "\033[0m", FMT, ##ARGS)
#define DRY_LOG_DEBUG(FMT, ARGS...) __DRY_LOG(DEBUG, "D", " \033[0;36m", "\033[0m", FMT, ##ARGS)
#define DRY_LOG_WARN(PPFMT, ARGS...) __DRY_LOG(WARN, "W", " \033[1;33m", "\033[0m", PPFMT, ##ARGS)
#define DRY_LOG_INFO(PPFMT, ARGS...) __DRY_LOG(INFO, "I", " ", "", PPFMT, ##ARGS)
#define DRY_LOG_ERROR(FMT, ARGS...) __DRY_LOG(ERROR, "E", " \033[1;31m", "\033[0m", FMT, ##ARGS)
