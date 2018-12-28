#ifndef _TIM_LOG_H_INCLUDE_
#define _TIM_LOG_H_INCLUDE_

#include <iostream>

#include <string>

#include <stdlib.h>

#include "muduo/base/LogFile.h"
#include "muduo/base/LogStream.h"
#include "muduo/base/Timestamp.h"
#include "muduo/base/TimeZone.h"
#include "boost/format.hpp"

namespace tim {

class Logger
{
public:
    enum SeverityLevel
    {
        TRACE,
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL,
        NUM_LOG_LEVELS
    };

    static void DoNothing() {}

    class SourceFile {
    public:
        SourceFile(const char* filename) : data_(file_name) {
            const char* slash = strrchr(filename, '/');
            if (slash)
            {
                data_ = slash + 1;
            }
            size_ = static_cast<int>(strlen(data_));
        }

    private:
        const char *data_;
        uint32_t size_;
    };

private:
    static SeverityLevel log_level_;

    static SeverityLevel InitLogLevel();

public:
    static void ResetLogLevel(SeverityLevel lv);

    static SeverityLevel LogLevel();

private:
    class Impl
    {
    public:
        Impl(SeverityLevel level, int old_errno, const SourceFile& file, int line);

        uint32_t time;
    
        SeverityLevel level;

        int line;

        SourceFile basename;

        void GetAndFormatTime();
        
        void Finish();
        muduo::LogStream stream_;
        muduo::Timestamp time_;
        SeverityLevel level_;
        int line_;
        SourceFile basename_;
    };

    Impl impl_;

public:
    muduo::LogStream& stream() { return impl_.stream_; }

    typedef void (*OutputFunc)(const char* msg, int len);

    typedef void (*FlushFunc)();

private:
    static OutputFunc output_;

    static FlushFunc flush_;

    static muduo::TimeZone log_time_zone_;

public:
    static void setOutput(OutputFunc);

    static void setFlush(FlushFunc);

    static void setTimeZone(const muduo::TimeZone& tz);
};

#define LOG_TRACE   (tim::Logger::LogLevel() <= tim::Logger::TRACE) ? \
                            tim::Logger::DoNothing() : tim::Logger(__FILE__, __LINE__, tim::Logger::TRACE).stream()
#define LOG_DEBUG   (tim::Logger::LogLevel() <= tim::Logger::DEBUG) ? \
                            tim::Logger::DoNothing() : tim::Logger(__FILE__, __LINE__, tim::Logger::DEBUG).stream()
#define LOG_INFO    (tim::Logger::LogLevel() <= tim::Logger::INFO) ? \
                            tim::Logger::DoNothing() : tim::Logger(__FILE__, __LINE__, tim::Logger::INFO).stream()
#define LOG_WARNING (tim::Logger::LogLevel() <= tim::Logger::WARNING) ? \
                            tim::Logger::DoNothing() : tim::Logger(__FILE__, __LINE__, tim::Logger::WARNING).stream()
#define LOG_ERROR   tim::Logger(__FILE__, __LINE__, tim::Logger::ERROR).stream()
#define LOG_FATAL   tim::Logger(__FILE__, __LINE__, tim::Logger::FATAL).stream()

// TODO
// Provide format log to old log expression.

int InitFileLogger(const char *filepath, const int& file_size = 200*1000);

}  // namespace tim

// Old log format;
void log(const char* fmt, ...);

#endif  // _TIM_LOG_H_INCLUDE_


