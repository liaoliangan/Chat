#ifndef APPLYINFO_H
#define APPLYINFO_H

#include <string>
#include <iostream>
struct ApplyInfo
{
    friend std::ostream &operator<<(std::ostream &os, const ApplyInfo &info);
    ApplyInfo(int uid, std::string name, std::string desc,
              std::string icon, std::string nick, int sex, int status)
        : _uid(uid), _name(name), _desc(desc),
          _icon(icon), _nick(nick), _sex(sex), _status(status) {}

    int _uid;
    std::string _name;
    std::string _desc;
    std::string _icon;
    std::string _nick;
    int _sex;
    int _status;
};

inline std::ostream &operator<<(std::ostream &os, const ApplyInfo &info)
{
    os << "uid:" << info._uid
       << " name:" << info._name
       << " desc:" << info._desc
       << " icon:" << info._icon
       << " nick:" << info._nick
       << " sex:" << info._sex
       << " status:" << info._status;
    return os;
}

#endif
