//
// Created by 33717 on 2026/1/20.
//

#include "SearchInfo.h"

#include <QDebug>

SearchInfo::SearchInfo(int uid, QString name, QString nick, QString desc, int sex, QString icon):
    _uid(uid), _name(name), _nick(nick), _desc(desc), _sex(sex), _icon(icon)
{
}

std::ostream& operator<<(std::ostream& os, const SearchInfo& si)
{
    {
        os << "uid: " << si._uid << " name: " << si._name.toStdString() << " nick: " << si._nick.toStdString() <<
            " desc: " << si._desc.toStdString() << " sex: " << si._sex << " icon: " << si._icon.toStdString();
        return os;
    }
}

QDebug operator<<(QDebug debug, const SearchInfo& si)
{
    QDebugStateSaver saver(debug); // 保存调试状态
    debug.nospace() << "SearchInfo(uid: " << si._uid
        << ", name: " << si._name
        << ", nick: " << si._nick
        << ", desc: " << si._desc
        << ", sex: " << si._sex
        << ", icon: " << si._icon << ")";
    return debug;
}
