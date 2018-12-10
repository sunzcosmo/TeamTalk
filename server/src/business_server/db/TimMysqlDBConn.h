#ifndef _TIM_MYSQL_DB_CONN_H_INCLUDE_
#define _TIM_MYSQL_DB_CONN_H_INCLUDE_

#include "../base/TimDBConn.h"

#include "mysql.h"
namespace mysql_tool {
    void mysql_bind(CResultSet *result_set, MYSQL_STMT* mysql_stmt);
}

class TimMysqlDBConn : public TimDBConn {
public:
    template<typename T>
    T* GetConn();
    
    bool TestDBConn(std::string& detail);

private:
    MYSQL *mysql_conn_;
 
};


#endif //_TIM_MYSQL_DB_CONN_H_INCLUDE_