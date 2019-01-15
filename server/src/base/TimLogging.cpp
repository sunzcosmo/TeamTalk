#include <TimLogging.h>

#include <muduo/base/CurrentThread.h>
#include <muduo/base/Timestamp.h>
#include <muduo/base/TimeZone.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <sstream>

#include <stdarg.h>

namespace tim {

  
const char* SeverityLevelName[tim::Logger::SeverityLevel::NUM_LOG_LEVELS] =
{
    "TRACE",
    "DEBUG",
    "INFO",
    "WARNING",
    "ERROR",
    "FATAL",
};

thread_local char t_errnobuf[512];
thread_local char t_time[64];
thread_local time_t t_lastSecond;

class T
{
 public:
  T(const char* str, unsigned len)
      : str_(str),
        len_(len)
  {
    assert(strlen(str) == len_);
  }

  const char* str_;
  const unsigned len_;
};

inline Logger::SeverityLevel Logger::InitLogLevel() {
    if       (::getenv("TIM_LOG_TRACE")) {
        return Logger::SeverityLevel::TRACE;
    } else if(::getenv("TIM_LOG_DEBUG")) {
        return Logger::SeverityLevel::DEBUG;
    } else if(::getenv("TIM_LOG_INFO")) {
        return Logger::SeverityLevel::INFO;
    } else if(::getenv("TIM_LOG_WARNING")) {
        return Logger::SeverityLevel::WARNING;
    } else if(::getenv("TIM_LOG_ERROR")) {
        return Logger::SeverityLevel::ERROR;
    }

    return Logger::SeverityLevel::INFO;
}

Logger::SeverityLevel Logger::log_level_ = InitLogLevel();

inline Logger::SeverityLevel LogLevel() {
    return Logger::log_level_;
}

// POXIS only
inline void Logger::ResetLogLevel(Logger::SeverityLevel lv) {
    LOG_FATAL<<"Reset log level, from ";
    if(unsetenv(SeverityLevelName[Logger::log_level_])) {
        LOG_ERROR<<"Reset log level failed."
        return;
    } else if(setenv(SeverityLevelName[lv], "", 0)) {
        LOG_ERROR<<"Reset log level failed."
        return;
    }
    Logger::log_level_ = InitLogLevel();
}

inline muduo::LogStream& operator<<(LogStream& s, T v)
{
  s.append(v.str_, v.len_);
  return s;
}

inline muduo::LogStream& operator<<(LogStream& s, const Logger::SourceFile& v)
{
  s.append(v.data_, v.size_);
  return s;
}

void DefaultOutput(const char* msg, int len)
{
  size_t n = fwrite(msg, 1, len, stdout);
  //FIXME check n
  (void)n;
}

void DefaultFlush()
{
  fflush(stdout);
}

Logger::OutputFunc Logger::output_ = DefaultOutput;
Logger::FlushFunc Logger::flush_ = DefaultFlush;

Logger::Impl::Impl(LogLevel level, int savedErrno, const SourceFile& file, int line)
    : time_(Timestamp::now()),
      stream_(),
      level_(level),
      line_(line),
      basename_(file)
{
  GetAndFormatTime();
  muduo::CurrentThread::tid();
  stream_ << T(CurrentThread::tidString(), CurrentThread::tidStringLength());
  stream_ << T(SeverityLevelName[level], 6);
  if (savedErrno != 0)
  {
    stream_ << strerror_tl(savedErrno) << " (errno=" << savedErrno << ") ";
  }
}

// muduo::Logger::Impl::formatTime()
void Logger::Impl::GetAndFormatTime()
{
  int64_t microSecondsSinceEpoch = time_.microSecondsSinceEpoch();
  time_t seconds = static_cast<time_t>(microSecondsSinceEpoch / Timestamp::kMicroSecondsPerSecond);
  int microseconds = static_cast<int>(microSecondsSinceEpoch % Timestamp::kMicroSecondsPerSecond);
  if (seconds != t_lastSecond)
  {
    t_lastSecond = seconds;
    struct tm tm_time;
    if (g_logTimeZone.valid())
    {
      tm_time = g_logTimeZone.toLocalTime(seconds);
    }
    else
    {
      ::gmtime_r(&seconds, &tm_time); // FIXME TimeZone::fromUtcTime
    }

    int len = snprintf(t_time, sizeof(t_time), "%4d%02d%02d %02d:%02d:%02d",
        tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
        tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    assert(len == 17); (void)len;
  }

  if (g_logTimeZone.valid())
  {
    muduo::Fmt us(".%06d ", microseconds);
    assert(us.length() == 8);
    stream_ << T(t_time, 17) << T(us.data(), 8);
  }
  else
  {
    muduo::Fmt us(".%06dZ ", microseconds);
    assert(us.length() == 9);
    stream_ << T(t_time, 17) << T(us.data(), 9);
  }
}

void Logger::Impl::finish()
{
  stream_ << " - " << basename_ << ':' << line_ << '\n';
}

Logger::Logger(Logger::SourceFile file, int line)
  : impl_(INFO, 0, file, line)
{
}

Logger::Logger(Logger::SourceFile file, int line, LogLevel level, const char* func)
  : impl_(level, 0, file, line)
{
  impl_.stream_ << func << ' ';
}

Logger::Logger(SourceFile file, int line, LogLevel level)
  : impl_(level, 0, file, line)
{
}

Logger::~Logger()
{
  impl_.finish();
  const muduo::LogStream::Buffer& buf(stream().buffer());
  Logger::output_(buf.data(), buf.length());
  if (impl_.level_ == FATAL)
  {
    Logger::flush_();
    abort();
  }
}

void Logger::setOutput(Logger::OutputFunc out)
{
  Logger::output_ = out;
}

void Logger::setFlush(Logger::FlushFunc flush)
{
  Logger::flush_ = flush;
}

void Logger::setTimeZone(const muduo::TimeZone& tz)
{
  Logger::log_time_zone_ = tz;
}

std::unique_ptr<muduo::LogFile> g_logFile;

namespace {
void OutputFunc(const char* msg, int len)
{
  g_logFile->append(msg, len);
}

void FlushFunc()
{
  g_logFile->flush();
}

} // annoymous namespace

using MAX_LOG_FILE_SIZE = 200*1024

int InitFileLogger(const char *filepath, const int& file_size)
{
    if(file_size > MAX_LOG_FILE_SIZE) {
        printf("The filesize %d is too large.", file_size);
        return -1;
    }
    g_logFile.reset(new muduo::LogFile(::basename(filepath), file_size));
    Logger::setOutput(OutputFunc);
    Logger::setFlush(FlushFunc);
    return 0;
}

} // namespace tim

using TIM_MAX_LOG_FORMAT_LEN 1024

void format_log(const char* fmt, ...) 
{
    size_t input_len = strlen(fmt);

    if(input_len > TIM_MAX_LOG_FORMAT_LEN) {
      LOG_ERROR<<"Input log format length is bigger than legal length.";
      return;
    }

    char log_out[TIM_MAX_LOG_FORMAT_LEN];
    
    va_list	args;
	  va_start(args, fmt);

    vsnprintf(log_out, TIM_MAX_LOG_FORMAT_LEN, fmt, args);

    LOG_INO<<log_out;
    
    va_end(args);
}