#ifndef MYSQLDAO_H
#define MYSQLDAO_H

#include <string>
#include <memory>
#include "MySqlPool.h"

class MysqlDao
{
public:
    MysqlDao();
    ~MysqlDao();
    int RegUser(const std::string &name, const std::string &email, const std::string &pwd);

private:
    std::unique_ptr<MySqlPool> pool_;
};

#endif
