#ifndef USERMGR_H
#define USERMGR_H
#include <QObject>
#include <memory>
#include <vector>
#include "ApplyInfo.h"
#include "singalton.h"
#include "UserInfo.h"

class UserMgr : public QObject, public Singalton<UserMgr>,
                public std::enable_shared_from_this<UserMgr>
{
    Q_OBJECT

public:
    friend class Singalton;
    ~UserMgr() override;
    void SetUserInfo(std::shared_ptr<UserInfo> user_info);
    void SetToken(QString token);
    QString GetName();
    QString GetNick();
    QString GetIcon();
    QString GetDesc();
    int GetUid();
    std::vector<std::shared_ptr<ApplyInfo>> GetApplyList();
    bool AlreadyApply(int uid);
    void AddApplyList(std::shared_ptr<ApplyInfo> app);
    void AppendFriendList(QJsonArray array);
    void AppendApplyList(QJsonArray array);
    bool CheckFriendById(int uid);
    void AddFriend(std::shared_ptr<AuthRsp> auth_rsp);
    void AddFriend(std::shared_ptr<AuthInfo> auth_info);
    std::shared_ptr<FriendInfo> GetFriendById(int uid);

private:
    UserMgr();
    std::shared_ptr<UserInfo> _user_info;
    QString _token;
    std::vector<std::shared_ptr<ApplyInfo>> _apply_list;
    std::vector<std::shared_ptr<FriendInfo>> _friend_list;
    QMap<int, std::shared_ptr<FriendInfo>> _friend_map;
};

#endif // USERMGR_H
