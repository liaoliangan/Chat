#ifndef USERINFO_H
#define USERINFO_H

#include <string>
#include <iostream>  // Changed from ostream to iostream to get std::ostream definition

struct UserInfo
{
    UserInfo() : name(""), pwd(""), uid(0), email(""), nick(""), desc(""), sex(0), icon(""), back("") {}
    std::string name;
    std::string pwd;
    int uid;
    std::string email;
    std::string nick;
    std::string desc;
    int sex;
    std::string icon;
    std::string back;
    
    // Declare operator<< as a friend function
    friend std::ostream &operator<<(std::ostream &os, const UserInfo &userInfo);
};

// Define the operator<< function outside the struct
inline std::ostream &operator<<(std::ostream &os, const UserInfo &userInfo)
{
    os << "name:" << userInfo.name
       << " pwd:" << userInfo.pwd
       << " uid:" << userInfo.uid
       << " email:" << userInfo.email
       << " nick:" << userInfo.nick
       << " desc:" << userInfo.desc
       << " sex:" << userInfo.sex
       << " icon:" << userInfo.icon
       << " back:" << userInfo.back;
    return os;
}

#endif