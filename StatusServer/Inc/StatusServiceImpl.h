#ifndef STATUS_SERVICE_IMPL_H
#define STATUS_SERVICE_IMPL_H

#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include <mutex>
#include <unordered_map>

class ChatServer
{
public:
    friend std::ostream &operator<<(std::ostream &os, const ChatServer &cs)
    {
        os << "ChatServer Info : " << "host:" << cs.host << " port:" << cs.port << " name:" << cs.name << " con_count:" << cs.con_count;
        return os;
    }
    ChatServer() : host(""), port(""), name(""), con_count(0) {}
    ChatServer(const ChatServer &cs) : host(cs.host), port(cs.port), name(cs.name), con_count(cs.con_count) {}
    ChatServer &operator=(const ChatServer &cs)
    {
        if (&cs == this)
        {
            return *this;
        }

        host = cs.host;
        name = cs.name;
        port = cs.port;
        con_count = cs.con_count;
        return *this;
    }
    std::string host;
    std::string port;
    std::string name;
    int con_count;
};

class StatusServiceImpl final : public message::StatusService::Service
{
public:
    StatusServiceImpl();
    grpc::Status GetChatServer(grpc::ServerContext *context, const message::GetChatServerReq *request,
                               message::GetChatServerRsp *reply) override;
    grpc::Status Login(grpc::ServerContext *context, const message::LoginReq *request,
                       message::LoginRsp *reply) override;

private:
    ChatServer getChatServer();
    void insertToken(int uid, std::string token);
    std::unordered_map<std::string, ChatServer> _servers;
    std::mutex _server_mtx;
};
#endif
