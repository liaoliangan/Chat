#ifndef STATUSSERVER_H
#define STATUSSERVER_H

#include "const.h"
#include "Singleton.h"
#include "ConfigMgr.h"
#include "message.grpc.pb.h"
#include <grpcpp/grpcpp.h>

class StatusConPool
{
public:
    StatusConPool(size_t poolsize, std::string host, std::string port);
    ~StatusConPool();
    void Close();
    std::unique_ptr<message::StatusService::Stub> GetConnection();
    void ReturnConnection(std::unique_ptr<message::StatusService::Stub> conn);

private:
    size_t poolSize_;
    std::string host_;
    std::string port_;
    std::mutex mutex_;
    std::atomic<bool> b_stop_;
    std::condition_variable cond_;
    std::queue<std::unique_ptr<message::StatusService::Stub>> connections_;
};

class StatusGrpcClient : public Singleton<StatusGrpcClient>
{
    friend class Singleton<StatusGrpcClient>;

public:
    ~StatusGrpcClient();
    message::GetChatServerRsp GetChatServer(int uid);
    message::LoginRsp Login(int uid, std::string token);

private:
    StatusGrpcClient();
    std::unique_ptr<StatusConPool> pool_;
};

#endif
