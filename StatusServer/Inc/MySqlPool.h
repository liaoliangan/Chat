#ifndef MYSQLPOOL_H
#define MYSQLPOOL_H

#include "const.h"
#include "Defer"
#include <thread>
#include <mysql-cppconn/jdbc/mysql_driver.h>
#include <mysql-cppconn/jdbc/mysql_connection.h>
#include <mysql-cppconn/jdbc/cppconn/prepared_statement.h>
#include <mysql-cppconn/jdbc/cppconn/statement.h>
#include <mysql-cppconn/jdbc/cppconn/exception.h>
#include <mysql-cppconn/jdbc/cppconn/resultset.h>

class SqlConnection
{
public:
    SqlConnection(sql::Connection *con, int64_t lasttime) : _con(con), _last_oper_time(lasttime) {}
    std::unique_ptr<sql::Connection> _con;
    int64_t _last_oper_time;
};

class MySqlPool
{
public:
    MySqlPool(const std::string &url, const std::string &user, const std::string &pass, const std::string &schema, int poolSize);
    void checkConnection();
    std::unique_ptr<SqlConnection> getConnection();
    void returnConnection(std::unique_ptr<SqlConnection> con);
    void Close();
    ~MySqlPool();

private:
    std::string url_;    // 数据库连接地址
    std::string user_;   // 数据库用户名
    std::string pass_;   // 数据库密码
    std::string schema_; // 数据库名
    int poolSize_;
    std::queue<std::unique_ptr<SqlConnection>> pool_; // 连接池
    std::mutex mutex_;
    std::condition_variable cond_;
    std::atomic<bool> b_stop_;
    std::thread _check_thread;
};

#endif
