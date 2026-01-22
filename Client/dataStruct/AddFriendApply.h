//
// Created by 33717 on 2026/1/22.
//

#ifndef ADDFRIENDAPPLY_H
#define ADDFRIENDAPPLY_H
#include<QString>

struct AddFriendApply
{
public:
    AddFriendApply(int from_uid, QString name, QString desc,
                   QString icon, QString nick, int sex): _from_uid(from_uid), _name(name),
                                                         _desc(desc), _icon(icon), _nick(nick), _sex(sex)
    {
    }

    int _from_uid;
    QString _name;
    QString _desc;
    QString _icon;
    QString _nick;
    int _sex;
};
#endif //ADDFRIENDAPPLY_H
