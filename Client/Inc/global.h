//
// Created by 33717 on 2026/1/10.
//

#ifndef GLOBAL_H
#define GLOBAL_H

#include<QWidget>
#include<functional>
#include<QRegularExpression>
#include<QNetworkReply>
#include<QStyle>
#include<memory>
#include<iostream>
#include<mutex>
#include<QMap>
#include<QJsonObject>
#include<QDir>
#include<QSettings>

#define COUT std::cout
#define ENDL std::endl

inline QString gate_url_prefix = "";

namespace LA
{
    extern std::function<void(QWidget*)> repolish; //刷新，使qss生效

    enum class ReqId
    {
        ID_GET_VARIFY_CODE = 1001, //获取验证码
        ID_REG_USER = 1002 //注册用户
    };

    enum class Modules
    {
        REGISTERMOD = 0 //注册模块
    };

    enum class ErrorCodes
    {
        SUCCESS = 0,
        ERR_JSON = 1, //json解析错误
        ERR_NETWORK = 2 //网络错误
    };
}

#endif //GLOBAL_H
