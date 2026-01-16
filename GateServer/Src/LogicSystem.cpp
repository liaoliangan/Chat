#include "LogicSystem.h"
#include "VarifyClient.h"
#include "HttpConnection.h"
#include "RedisMgr.h"
#include "MysqlMgr.h"
void LogicSystem::RegGet(std::string url, HttpHandler handler)
{
    _get_handlers.insert(make_pair(url, handler));
}

void LogicSystem::RegPost(std::string url, HttpHandler handler)
{
    _post_handlers.insert(make_pair(url, handler));
}

LogicSystem::LogicSystem()
{
    RegPost("/get_varifycode", [](std::shared_ptr<HttpConnection> connection)
            {
        std::string body_str=boost::beast::buffers_to_string(connection->_request.body().data());
        std::cout << "receive post req" << std::endl;
        connection->_response.set(boost::beast::http::field::content_type, "application/json");
        Json::Value root;
        Json::Reader reader;
        Json::Value src_root;
        bool parse_success = reader.parse(body_str, src_root);
        if(!parse_success)
        {
            std::cout<<"Failed to parse JSON"<<std::endl;
            root["error"] = static_cast<int>(LA::ErrorCodes::ERROR_JSON);
            root["msg"] = "Invalid JSON format";
            std::string jsonstr=root.toStyledString();
            boost::beast::ostream(connection->_response.body()) << jsonstr;
            return true;
        }

        if(!src_root.isMember("email"))
        {
            std::cout<<"Failed to parse JSON"<<std::endl;
            root["error"] = static_cast<int>(LA::ErrorCodes::ERROR_JSON);
            root["msg"] = "Invalid JSON format";
            std::string jsonstr=root.toStyledString();
            boost::beast::ostream(connection->_response.body()) << jsonstr;
            return true;
        }

        auto email = src_root["email"].asString();
        message::GetVarifyRsp rsp=VarifyClient::GetInstance()->GetVarifyCode(email);

        std::cout << "email is " << email << std::endl;
        root["error"] = rsp.error();
        root["email"] = src_root["email"];
        std::string jsonstr=root.toStyledString();
        boost::beast::ostream(connection->_response.body()) << jsonstr;
        return true; });
    RegPost("/user_register", [](std::shared_ptr<HttpConnection> connection)
            {
    auto body_str = boost::beast::buffers_to_string(connection->_request.body().data());
    std::cout << "receive body is " << body_str << std::endl;
    connection->_response.set(boost::beast::http::field::content_type, "text/json");
    Json::Value root;
    Json::Reader reader;
    Json::Value src_root;
    bool parse_success = reader.parse(body_str, src_root);
    if (!parse_success) {
        std::cout << "Failed to parse JSON data!" << std::endl;
        root["error"] = static_cast<int>(LA::ErrorCodes::ERROR_JSON);
        std::string jsonstr = root.toStyledString();
        boost::beast::ostream(connection->_response.body()) << jsonstr;
        return true;
    }

    auto email=src_root["email"].asString();
    auto name=src_root["user_name"].asString();
    auto pwd=src_root["passwd"].asString();
    auto confirm=src_root["confirm"].asString();

    if(pwd != confirm)
    {
        std::cout<<"password err "<<std::endl;
        root["error"]=static_cast<int>(LA::ErrorCodes::PASSWD_ERROR);
        std::string jsonstr=root.toStyledString();
        boost::beast::ostream(connection->_response.body()) << jsonstr;
        return true;
    }

    //先查找redis中email对应的验证码是否合理
    std::string  varify_code;
    bool b_get_varify = RedisMgr::GetInstance()->Get(CODEPREFIX + src_root["email"].asString(), varify_code);
    if (!b_get_varify) {
        std::cout << " get varify code expired" << std::endl;
        root["error"] = static_cast<int>(LA::ErrorCodes::VARIFY_EXPIRED);
        std::string jsonstr = root.toStyledString();
        boost::beast::ostream(connection->_response.body()) << jsonstr;
        return true;
    }

    if (varify_code != src_root["varifycode"].asString()) {
        std::cout << " varify code error" << std::endl;
        root["error"] = static_cast<int>(LA::ErrorCodes::VARIFY_CODE_ERROR);
        std::string jsonstr = root.toStyledString();
        boost::beast::ostream(connection->_response.body()) << jsonstr;
        return true;
    }

    //访问redis查找
    //TODO

    //查找数据库判断用户是否存在
    int uid = MysqlMgr::GetInstance()->RegUser(name, email, pwd);
    if (uid == 0 || uid == -1) {
        std::cout << " user or email exist" << std::endl;
        root["error"] = static_cast<int>(LA::ErrorCodes::USER_EXIST);
        std::string jsonstr = root.toStyledString();
        boost::beast::ostream(connection->_response.body()) << jsonstr;
        return true;
    }

    root["error"] = 0;
    root["uid"]=uid;
    root["email"] = src_root["email"];
    root["user"]= src_root["user"].asString();
    root["passwd"] = src_root["passwd"].asString();
    root["confirm"] = src_root["confirm"].asString();
    root["varifycode"] = src_root["varifycode"].asString();
    std::string jsonstr = root.toStyledString();
    boost::beast::ostream(connection->_response.body()) << jsonstr;
    return true; });

    RegPost("/reset_pwd", [](std::shared_ptr<HttpConnection> connection)
            {
    auto body_str = boost::beast::buffers_to_string(connection->_request.body().data());
    std::cout << "receive body is " << body_str << std::endl;
    connection->_response.set(boost::beast::http::field::content_type, "text/json");
    Json::Value root;
    Json::Reader reader;
    Json::Value src_root;
    bool parse_success = reader.parse(body_str, src_root);
    if (!parse_success) {
        std::cout << "Failed to parse JSON data!" << std::endl;
        root["error"] = static_cast<int>(LA::ErrorCodes::ERROR_JSON);
        std::string jsonstr = root.toStyledString();
        boost::beast::ostream(connection->_response.body()) << jsonstr;
        return true;
    }

    auto email = src_root["email"].asString();
    auto name = src_root["user"].asString();
    auto pwd = src_root["passwd"].asString();

    //先查找redis中email对应的验证码是否合理
    std::string  varify_code;
    bool b_get_varify = RedisMgr::GetInstance()->Get(CODEPREFIX + src_root["email"].asString(), varify_code);
    if (!b_get_varify) {
        std::cout << " get varify code expired" << std::endl;
        root["error"] = static_cast<int>(LA::ErrorCodes::VARIFY_EXPIRED);
        std::string jsonstr = root.toStyledString();
        boost::beast::ostream(connection->_response.body()) << jsonstr;
        return true;
    }

    if (varify_code != src_root["varifycode"].asString()) {
        std::cout << " varify code error" << std::endl;
        root["error"] = static_cast<int>(LA::ErrorCodes::VARIFY_CODE_ERROR);
        std::string jsonstr = root.toStyledString();
        boost::beast::ostream(connection->_response.body()) << jsonstr;
        return true;
    }
    //查询数据库判断用户名和邮箱是否匹配
    bool email_valid = MysqlMgr::GetInstance()->CheckEmail(name, email);
    if (!email_valid) {
        std::cout << " user email not match" << std::endl;
        root["error"] = static_cast<int>(LA::ErrorCodes::EMAIL_NOT_MATCH);
        std::string jsonstr = root.toStyledString();
        boost::beast::ostream(connection->_response.body()) << jsonstr;
        return true;
    }

    //更新密码为最新密码
    bool b_up = MysqlMgr::GetInstance()->UpdatePwd(name, pwd);
    if (!b_up) {
        std::cout << " update pwd failed" << std::endl;
        root["error"] = static_cast<int>(LA::ErrorCodes::PASSWD_UPDATE_FAILED);
        std::string jsonstr = root.toStyledString();
        boost::beast::ostream(connection->_response.body()) << jsonstr;
        return true;
    }

    std::cout << "succeed to update password" << pwd << std::endl;
    root["error"] = 0;
    root["email"] = email;
    root["user"] = name;
    root["passwd"] = pwd;
    root["varifycode"] = src_root["varifycode"].asString();
    std::string jsonstr = root.toStyledString();
    boost::beast::ostream(connection->_response.body()) << jsonstr;
    return true; });
}
LogicSystem::~LogicSystem() = default;
bool LogicSystem::HandleGet(std::string path, std::shared_ptr<HttpConnection> con)
{
    if (_get_handlers.find(path) == _get_handlers.end())
    {
        return false;
    }

    _get_handlers[path](con);
    return true;
}

bool LogicSystem::HandlePost(std::string path, std::shared_ptr<HttpConnection> con)
{
    if (_post_handlers.find(path) == _post_handlers.end())
    {
        return false;
    }

    _post_handlers[path](con);
    return true;
}
