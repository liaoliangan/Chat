//
// Created by 33717 on 2026/1/20.
//

#ifndef SEARCHINFO_H
#define SEARCHINFO_H

#include<QString>
#include<ostream>

class SearchInfo
{
public:
    friend std::ostream& operator<<(std::ostream& os, const SearchInfo& si);
    friend QDebug operator<<(QDebug debug, const SearchInfo& si);
    SearchInfo(int uid, QString name, QString nick, QString desc, int sex);
    int _uid;
    QString _name;
    QString _nick;
    QString _desc;
    int _sex;
};

std::ostream& operator<<(std::ostream& os, const SearchInfo& si);
QDebug operator<<(QDebug debug, const SearchInfo& si);
#endif //SEARCHINFO_H
