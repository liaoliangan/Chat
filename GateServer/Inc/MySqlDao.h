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
    bool UpdatePwd(const std::string& name, const std::string& newpwd);
    bool CheckEmail(const std::string& name, const std::string& email);
private:
    std::unique_ptr<MySqlPool> pool_;
};

#endif
