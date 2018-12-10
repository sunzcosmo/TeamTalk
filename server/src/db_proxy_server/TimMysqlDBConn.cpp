#include "../base/TimConfig.h"

#include "TimMysqlDBConn.h"

namespace mysql_tool {
    bool mysql_bind(CResultSet *result_set, MYSQL_STMT* mysql_stmt) 
    {
        if(nullptr == result_set || nullptr == mysql_stmt) {
            return false;
        }

        MYSQL_RES *metadata = mysql_stmt_result_metadata(mysql_stmt_);
        int result_size = mysql_num_fields(metadata);
        result_set->result_size_ = result_size;
        result_set->result_binds_ =  
        (MYSQL_BIND*)calloc(result_size, sizeof(MYSQL_BIND));
        result_array_ = 
        (BYTE**)calloc(result_size, sizeof(BYTE*));
        MYSQL_FIELD* fields = mysql_fetch_fields(metadata);
        for(int i=0; i<result_size; ++i)
        {
        result_binds_[i].buffer_type = fields[i].type;
        uint32_t buffer_length = fields[i].length;
        result_binds_[i].buffer_length = buffer_length;
        result_array_[i] = 
            (BYTE*)calloc(buffer_length, sizeof(BYTE));
        result_binds_[i].buffer = result_array_[i];
        fields_map_.insert(
            std::make_pair(string(fields[i].name), i));
        }
        mysql_stmt_bind_result(mysql_stmt_, result_binds_);
        mysql_free_result(metadata);
    }
}

<template typename T>
T* TimMysqlDBConn::GetConn() 
{
    return this->mysql_conn_;
}

bool TimMysqlDBConn::TestDBConn(std::string& detail)
{
    tim_int_t mysql_connection_status = mysql_ping(this->mysql_conn_);

	bool ret = false;   
	switch mysql_connection_status {
		case 0:
            ret = true;
		break;
		case CR_COMMANDS_OUT_OF_SYNC:
            detail = "Mysql error: Commands were executed in an improper order.";
			ret = false;
		break;
        case CR_SERVER_GONE_ERROR:
            detail ="Mysql error: The MySQL server has gone away.")
			ret = false;
		break;
		case CR_UNKNOWN_ERROR:
		default:
            detail = "Mysql error: An unknown error occurred.";
			ret = false;
		break;
	}
	return ret;
}

