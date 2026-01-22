#include "UserMgr.h"

UserMgr::~UserMgr()
{
    qDebug()<<"~UserMgr()";
}

void UserMgr::SetUserInfo(std::shared_ptr<UserInfo> user_info) {
    _user_info = user_info;
}

void UserMgr::SetToken(QString token)
{
    _token = token;
}

int UserMgr::GetUid()
{
    return _user_info->_uid;
}

QString UserMgr::GetName()
{
    qDebug()<<"UserMgr::GetName: "<<_user_info->_name;
    return _user_info->_name;
}

QString UserMgr::GetNick()
{
    return _user_info->_nick;
}

QString UserMgr::GetIcon()
{
    return _user_info->_icon;
}

QString UserMgr::GetDesc()
{
    return _user_info->_desc;
}

UserMgr::UserMgr()
{
    qDebug()<<"UserMgr()";
}
std::vector<std::shared_ptr<ApplyInfo> > UserMgr::GetApplyList()
{
    return _apply_list;
}