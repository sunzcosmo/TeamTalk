#include "../TimLogging.h"

#include <iostream>

using namespace tim;

int main(const int argc, const char* const argv[])
{
    if(-1 == InitFileLogger("./test.log", 1024*200))
    {
        std::cout<<"[ERROR] Init test.log failed.\n";
    }

    LOG_TRACE<<"Test LOG_TRACE, SeverityLevel: default";
    LOG_DEBUG<<"Test LOG_DEBUG, SeverityLevel: default";
    LOG_INFO<<"Test LOG_INFO, SeverityLevel: default";
    LOG_WARNING<<"Test LOG_WARNING, SeverityLevel: default";
    LOG_ERROR<<"Test LOG_ERROR, SeverityLevel: default";
    LOG_FATAL<<"Test LOG_FATAL, SeverityLevel: default";

    Logger::ResetLogLevel(Logger::SeverityLevel::DEBUG);

    LOG_TRACE<<"Test LOG_TRACE, SeverityLevel: " << SeverityLevelName[Logger::LogLevel()];
    LOG_DEBUG<<"Test LOG_DEBUG, SeverityLevel: " << SeverityLevelName[Logger::LogLevel()];
    LOG_INFO<<"Test LOG_INFO, SeverityLevel: " << SeverityLevelName[Logger::LogLevel()];
    LOG_WARNING<<"Test LOG_WARNING, SeverityLevel: " << SeverityLevelName[Logger::LogLevel()];
    LOG_ERROR<<"Test LOG_ERROR, SeverityLevel: " << SeverityLevelName[Logger::LogLevel()];
    LOG_FATAL<<"Test LOG_FATAL, SeverityLevel: " << SeverityLevelName[Logger::LogLevel()];

    log("Test old log function: str=%s, num=%d", "0123456789 acbcdefghijklmnopqrstuvwxyz;", 1024)

    return 0;
}

