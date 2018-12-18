#ifndef _TIM_LOG_H_INCLUDE_
#define _TIM_LOG_H_INCLUDE_

#include <iostream>

#include <string>

#include <stdlib.h>


namespace tim {

class TimLogFile {

};

class TimLogger
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

    const char* SeverityLevelName[NUM_LOG_LEVELS] =
    {
        "TRACE",
        "DEBUG",
        "INFO",
        "WARNING",
        "ERROR",
        "FATAL",
    };

    static void DoNothing() {}

    class SourceFile {
        SourceFile(const char* file_name);
    };

private:
    static SeverityLevel log_level_;

    static SeverityLevel InitLogLevel();

    static void TimLogger::ResetLogLevel(TimLogger::SeverityLevel lv);

public:

    void UpdateLogLevel(SeverityLevel level);

private:
    class Impl
    {
    public:
        Impl(SeverityLevel level, int old_errno, const TimLogger::SourceFile& file, int line);

        uint32_t time;
        
        // LogStream stream_;
        SeverityLevel level;

        int line;

        SourceFile basename;

        void _GetAndFormatTime();
        
        void Finish();
    };

    Impl impl_;
};

TimLogger::SeverityLevel TimLogger::log_level_ = InitLogLevel();

inline TimLogger::SeverityLevel TimLogger::InitLogLevel() {
    if(::getenv("TIM_LOG_TRACE")) {
        return TimLogger::SeverityLevel::TRACE;
    } else if(::getenv("TIM_LOG_DEBUG")) {
        return TimLogger::SeverityLevel::DEBUG;
    } else if(::getenv("TIM_LOG_INFO")) {
        return TimLogger::SeverityLevel::INFO;
    } else if(::getenv("TIM_LOG_WARNING")) {
        return TimLogger::SeverityLevel::WARNING;
    } else if(::getenv("TIM_LOG_ERROR")) {
        return TimLogger::SeverityLevel::ERROR;
    }

    return TimLogger::SeverityLevel::INFO;
}

// TODO
// Support format sytanx
#define LOG_TRACE   (tim::logger::logLevel() > tim::logger::TRACE) ? \
                            TimLogger::DoNothing() : tim::logger::TimLogger(__FILE__, __LINE__, tim::logger::TRACE).stream()
#define LOG_DEBUG   (tim::logger::logLevel() > tim::logger::DEBUG) ? \
                            TimLogger::DoNothing() : tim::logger::TimLogger(__FILE__, __LINE__, tim::logger::DEBUG).stream()
#define LOG_INFO    (tim::logger::logLevel() > tim::logger::INFO) ? \
                            TimLogger::DoNothing() : tim::logger::TimLogger(__FILE__, __LINE__, tim::Logger::INFO).stream()
#define LOG_WARNING (tim::logger::logLevel() > tim::logger::WARNING) ? \
                            TimLogger::DoNothing() : tim::logger::TimLogger(__FILE__, __LINE__, tim::Logger::WARNING).stream()
#define LOG_ERROR   tim::TimLogger(__FILE__, __LINE__, tim::Logger::ERROR).stream()
#define LOG_FATAL   tim::TimLogger(__FILE__, __LINE__, tim::Logger::FATAL).stream()

// POXIS only
inline void TimLogger::ResetLogLevel(TimLogger::SeverityLevel lv) {
    LOG_FATAL<<"Reset log level, from ";
    if(unsetenv(SeverityLevelName[TimLogger::log_level_])) {
        LOG_ERROR<<"Reset log level failed."
    } else if(setenv(SeverityLevelName[lv], "", 0)) {
        LOG_ERROR<<"Reset log level failed."
    }

    TimLogger::log_level_ = InitLogLevel();
}

}  // namespace tim

#endif  // _TIM_LOG_H_INCLUDE_


