#ifndef _TIM_LOG_HPP_INCLUDE_
#define _TIM_LOG_HPP_INCLUDE_

#include <iostream>

#include <boost/log/core/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/trivial.hpp>

namespace tim {

namespace log {

enum severity_level
{
    TRACE,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

class TimLog {
public:
    TimLog();

    ~TimLog();

    void Init();

    void SetFilter(severity_level lv = tim::log::INFO);
private:
    void _AddFileLog();
};

TimLog::TimLog() 
{
    Init();
}

void TimLog::Init() {

}

void TimLog::SetFilter(severity_level lv) {
}

    
}  // namespace log

}  // namespace tim

#endif  // _TIM_LOG_HPP_INCLUDE_


