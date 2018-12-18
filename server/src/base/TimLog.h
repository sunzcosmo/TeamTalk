#ifndef _TIM_LOG_H_INCLUDE_
#define _TIM_LOG_H_INCLUDE_

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

    TimLog *GetInstance();

    void Init();

    void SetFilter(severity_level lv = tim::log::INFO);
private:
    ~TimLog();

    static TimLog *_singleton;

    void _AddFileLog();
};

TimLog::_singleton = new TimLog();

TimLog::TimLog() 
{
    this->Init();

    boost::log::add_common_attributes();

    boost::log::sources::severity_logger<severity_level> lg;

    BOOST_LOG_SEV(lg, INFO) << "An informational severity message";
}

void TimLog::Init() {

    boost::log::add_file_log("test.log");

    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info);
}
 
void TimLog::SetFilter(severity_level lv) {
    boost::log::core::get()->set_filter(true);
}

    
}  // namespace log

}  // namespace tim

#endif  // _TIM_LOG_H_INCLUDE_


