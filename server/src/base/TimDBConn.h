#ifndef _TIM_DB_CONN_H_INCLUDE_
#define _TIM_DB_CONN_H_INCLUDE_

#include <string>

#include "../base/TimConfig.h"

class TimDBConn
{
public:
    TimDBConn();

    TimDBConn(const TimDBConn& left) = delete;

    virtual ~TimDBConn();

    template<typename T>
    T* GetConn() = 0;

    virtual bool TestDBConn(std::string& detail) = 0;

private:
    std::string db_host_;
    tim_uint_t db_port_;
    std::string username_;
    std::string passwd_;
    std::string db_name_;
};


#endif //_TIM_DB_CONN_H_INCLUDE_