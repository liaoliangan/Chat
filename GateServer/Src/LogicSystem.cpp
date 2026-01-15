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
    RegGet("/get_test", [](std::shared_ptr<HttpConnection> connection)
           {
        boost::beast::ostream(connection->_response.body()) << "receive get_test req"<<std::endl;
        int i=0;
        for(auto& elem:connection->_get_params)
        {
            i++;
            boost::beast::ostream(connection->_response.body()) << "param " << i << ": key is " << elem.first << ":\t value is " << elem.second << std::endl;
        } });

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
