//
// Created by 33717 on 2026/1/20.
//

#ifndef SEARCHINFO_H
#define SEARCHINFO_H

#include<QString>

class SearchInfo
{
public:
    SearchInfo(int uid, QString name, QString nick, QString desc, int sex);
    int _uid;
    QString _name;
    QString _nick;
    QString _desc;
    int _sex;
};


#endif //SEARCHINFO_H
