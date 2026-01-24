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
const int tip_offset = 5;

//申请好友标签输入框最低长度
const int MIN_APPLY_LABEL_ED_LEN = 40;
const QString add_prefix = "添加标签 ";

//TODO 测试数据,记得删
//----------------------------------------------------------------------------
const std::vector<QString> strs = {
    "hello world !",
    "nice to meet u",
    "New year，new life",
    "You have to love yourself",
    "My love is written in the wind ever since the whole world is you"
};

const std::vector<QString> heads = {
    ":/image/head_1.jpg",
    ":/image/head_2.jpg",
    ":/image/head_3.jpg",
    ":/image/head_4.jpg",
    ":/image/head_5.jpg"
};

const std::vector<QString> names = {
    "HanMeiMei",
    "Lily",
    "Ben",
    "Androw",
    "Max",
    "Summer",
    "Candy",
    "Hunter"
};
//----------------------------------------------------------------------------

namespace LA
{
    extern std::function<void(QWidget*)> repolish; //刷新，使qss生效

    enum class ReqId
    {
        ID_GET_VARIFY_CODE = 1001, //获取验证码
        ID_REG_USER = 1002, //注册用户
        ID_RESET_PWD = 1003, //重置密码
        ID_LOGIN_USER = 1004, //登录用户
        ID_CHAT_LOGIN = 1005, //聊天登录
        ID_CHAT_LOGIN_RSP = 1006, //聊天登录响应
        ID_SEARCH_USER_REQ = 1007, //用户搜索请求
        ID_SEARCH_USER_RSP = 1008, //搜索用户回包
        ID_ADD_FRIEND_REQ = 1009, //添加好友申请
        ID_ADD_FRIEND_RSP = 1010, //申请添加好友回复
        ID_NOTIFY_ADD_FRIEND_REQ = 1011, //通知用户添加好友申请
        ID_AUTH_FRIEND_REQ = 1013, //认证好友请求
        ID_AUTH_FRIEND_RSP = 1014, //认证好友回复
        ID_NOTIFY_AUTH_FRIEND_REQ = 1015, //通知用户认证好友申请
        ID_TEXT_CHAT_MSG_REQ = 1017, //文本聊天信息请求
        ID_TEXT_CHAT_MSG_RSP = 1018, //文本聊天信息回复
        ID_NOTIFY_TEXT_CHAT_MSG_REQ = 1019, //通知用户文本聊天信息
        ID_NOTIFY_OFF_LINE_REQ = 1021, //通知用户下线
        ID_HEART_BEAT_REQ = 1023, //心跳请求
        ID_HEARTBEAT_RSP = 1024, //心跳回复
    };

    enum class Modules
    {
        REGISTERMOD = 0, //注册模块
        RESETMOD = 1, //重置密码模块
        LOGINMOD = 2, //登录模块
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
        PASSWD_INVALID = 1009, // 密码无效
        TOKEN_INVALIED = 1010, //Token失效
        UID_INVALIED = 1011, //uid无效
    };

    enum class TipErr
    {
        TIP_SUCCESS = 0, //成功
        TIP_EMAIL_ERR = 1, //邮箱格式错误
        TIP_PWD_ERR = 2, //密码格式错误
        TIP_CONFIRM_ERR = 3, //确认密码格式错误
        TIP_PWD_CONFIRM = 4, //密码不一致
        TIP_VARIFY_ERR = 5, //验证码格式错误
        TIP_USER_ERR = 6 //用户名格式错误
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

enum ChatUIMode
{
    SearchMode, //搜索模式
    ChatMode, //聊天模式
    ContactMode //联系人模式
};

enum ListItemType
{
    CHAT_USER_ITEM, //聊天用户
    CONTACT_USER_ITEM, //联系人用户
    SEARCH_USER_ITEM, //搜索到的用户
    ADD_USER_TIP_ITEM, //提示添加用户
    INVALID_ITEM, //不可点击条目
    GROUP_TIP_ITEM, //分组提示条目
    LINE_ITEM, //分割线
    APPLY_FRIEND_ITEM, //好友申请
};

enum ChatRole
{
    Self,
    Other
};

#endif //GLOBAL_H
