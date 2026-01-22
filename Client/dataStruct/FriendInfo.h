//
// Created by 33717 on 2026/1/21.
//

#ifndef FRIENDINFO_H
#define FRIENDINFO_H

#include <QString>
#include "AuthInfo.h"
#include "AuthRsp.h"

struct TextChatData;
struct FriendInfo {
    FriendInfo(int uid, QString name, QString nick, QString icon,
        int sex, QString desc, QString back, QString last_msg=""):_uid(uid),
        _name(name),_nick(nick),_icon(icon),_sex(sex),_desc(desc),
        _back(back),_last_msg(last_msg){}

    FriendInfo(std::shared_ptr<AuthInfo> auth_info):_uid(auth_info->_uid),
    _nick(auth_info->_nick),_icon(auth_info->_icon),_name(auth_info->_name),
      _sex(auth_info->_sex){}

    FriendInfo(std::shared_ptr<AuthRsp> auth_rsp):_uid(auth_rsp->_uid),
    _nick(auth_rsp->_nick),_icon(auth_rsp->_icon),_name(auth_rsp->_name),
      _sex(auth_rsp->_sex){}

    void AppendChatMsgs(const std::vector<std::shared_ptr<TextChatData>> text_vec);

    int _uid;
    QString _name;
    QString _nick;
    QString _icon;
    int _sex;
    QString _desc;
    QString _back;
    QString _last_msg;
    std::vector<std::shared_ptr<TextChatData>> _chat_msgs;
};

#endif //FRIENDINFO_H
