#ifndef USERMGR_H
#define USERMGR_H
#include <QObject>
#include <memory>
#include <vector>
#include "ApplyInfo.h"
#include "singalton.h"
#include "UserInfo.h"
class UserMgr:public QObject,public Singalton<UserMgr>,
        public std::enable_shared_from_this<UserMgr>
{
    Q_OBJECT
public:
    friend class Singalton;
    ~UserMgr();
    void SetUserInfo(std::shared_ptr<UserInfo> user_info);
    void SetToken(QString token);
    QString GetName();
    QString GetNick();
    QString GetIcon();
    QString GetDesc();
    int GetUid();
    std::vector<std::shared_ptr<ApplyInfo>> GetApplyList();
private:
    UserMgr();
    std::shared_ptr<UserInfo> _user_info;
    QString _token;
    std::vector<std::shared_ptr<ApplyInfo>> _apply_list;
};

#endif // USERMGR_H