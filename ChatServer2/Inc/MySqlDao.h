#ifndef MYSQLDAO_H
#define MYSQLDAO_H

#include <string>
#include <memory>
#include "MySqlPool.h"
#include "data.h"
class MysqlDao
{
public:
    MysqlDao();
    ~MysqlDao();
    int RegUser(const std::string &name, const std::string &email, const std::string &pwd);
    bool UpdatePwd(const std::string& name, const std::string& newpwd);
    bool CheckEmail(const std::string& name, const std::string& email);
    bool CheckPwd(const std::string& name, const std::string& pwd, UserInfo& userInfo);
    std::shared_ptr<UserInfo> GetUser(int uid);
	std::shared_ptr<UserInfo> GetUser(std::string name);
    bool AddFriendApply(const int& from, const int& to);
	bool AuthFriendApply(const int& from, const int& to);
	bool AddFriend(const int& from, const int& to, std::string back_name);
	bool GetApplyList(int touid, std::vector<std::shared_ptr<ApplyInfo>>& applyList, int offset, int limit );
	bool GetFriendList(int self_id, std::vector<std::shared_ptr<UserInfo> >& user_info);
private:
    std::unique_ptr<MySqlPool> pool_;
};

#endif
