#ifndef CONST_H
#define CONST_H

#include <queue>
#include <memory>
#include <atomic>
#include <iostream>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <condition_variable>
#include <boost/beast/http.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#define CODEPREFIX "code_"

namespace LA
{
    enum class ErrorCodes
    {
        SUCCESS = 0,                 // 成功
        ERROR_JSON = 1001,           // json解析错误
        RPCFAILED = 1002,            // rpc调用失败
        VARIFY_EXPIRED = 1003,       // 验证码已过期
        VARIFY_CODE_ERROR = 1004,    // 验证码错误
        USER_EXIST = 1005,           // 用户已存在
        PASSWD_ERROR = 1006,         // 密码错误
        EMAIL_NOT_MATCH = 1007,      // 邮箱不匹配
        PASSWD_UPDATE_FAILED = 1008, // 密码更新失败
        PASSWD_INVALID               // 密码无效
    };
};
#endif
