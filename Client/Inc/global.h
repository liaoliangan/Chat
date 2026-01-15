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
#include<QCryptographicHash>
#include<QByteArray>

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
        SUCCESS = 0, // 成功
        ERROR_JSON = 1001, // json解析错误
        RPCFAILED = 1002, // rpc调用失败
        VARIFY_EXPIRED = 1003, // 验证码已过期
        VARIFY_CODE_ERROR = 1004, // 验证码错误
        USER_EXIST = 1005, // 用户已存在
        PASSWD_ERROR = 1006, // 密码错误
        EMAIL_NOT_MATCH = 1007, // 邮箱不匹配
        PASSWD_UPDATE_FAILED = 1008, // 密码更新失败
        PASSWD_INVALID // 密码无效
    };

    enum class TipErr
    {
        TIP_SUCCESS = 0,
        TIP_EMAIL_ERR = 1,
        TIP_PWD_ERR = 2,
        TIP_CONFIRM_ERR = 3,
        TIP_PWD_CONFIRM = 4,
        TIP_VARIFY_ERR = 5,
        TIP_USER_ERR = 6
    };

    static std::function<QString(QString)> md5Hash = [](QString input)-> QString
    {
        QByteArray input_byte = input.toUtf8();
        QByteArray output_byte = QCryptographicHash::hash(input_byte, QCryptographicHash::Md5);
        return QString(output_byte.toHex());
    };

    enum class ClickLbState
    {
        Normal = 0, //闭眼状态
        Selected = 1 //睁眼状态
    };
}

#endif //GLOBAL_H
