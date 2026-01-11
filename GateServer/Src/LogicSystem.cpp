#include "LogicSystem.h"
#include "HttpConnection.h"
#include "const.h"

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
        std::cout << "email is " << email << std::endl;
        root["error"] = static_cast<int>(LA::ErrorCodes::SUCCESS);
        root["email"] = src_root["email"];
        std::string jsonstr=root.toStyledString();
        boost::beast::ostream(connection->_response.body()) << jsonstr;
        return true;
    });
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
