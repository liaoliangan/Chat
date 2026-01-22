//
// Created by 33717 on 2026/1/22.
//

#ifndef APPLYINFO_H
#define APPLYINFO_H
#include<QString>
#include "AddFriendApply.h"

struct ApplyInfo
{
    ApplyInfo(int uid, QString name, QString desc,
              QString icon, QString nick, int sex, int status)
        : _uid(uid), _name(name), _desc(desc),
          _icon(icon), _nick(nick), _sex(sex), _status(status)
    {
    }

    ApplyInfo(std::shared_ptr<AddFriendApply> addinfo)
        : _uid(addinfo->_from_uid), _name(addinfo->_name),
          _desc(addinfo->_desc), _icon(addinfo->_icon),
          _nick(addinfo->_nick), _sex(addinfo->_sex),
          _status(0)
    {
    }

    void SetIcon(QString head)
    {
        _icon = head;
    }

    int _uid;
    QString _name;
    QString _desc;
    QString _icon;
    QString _nick;
    int _sex;
    int _status;
};

#endif //APPLYINFO_H
