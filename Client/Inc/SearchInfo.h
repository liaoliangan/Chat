//
// Created by 33717 on 2026/1/20.
//

#ifndef SEARCHINFO_H
#define SEARCHINFO_H

#include<QString>

class SearchInfo
{
public:
    friend std::ostream& operator<<(std::ostream& os, const SearchInfo& si);
    friend QDebug operator<<(QDebug debug, const SearchInfo& si);
    SearchInfo(int uid, QString name, QString nick, QString desc, int sex, QString icon);
    int _uid;
    int _sex;
    QString _name;
    QString _nick;
    QString _desc;
    QString _icon;
};

std::ostream& operator<<(std::ostream& os, const SearchInfo& si);
QDebug operator<<(QDebug debug, const SearchInfo& si);
#endif //SEARCHINFO_H
