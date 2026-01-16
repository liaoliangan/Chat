#ifndef STATUS_SERVICE_IMPL_H
#define STATUS_SERVICE_IMPL_H

#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include <mutex>
#include <unordered_map>

class ChatServer
{
public:
    std::string host;
    std::string port;
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
    std::vector<ChatServer> _servers;
    int _server_index;
};
#endif
