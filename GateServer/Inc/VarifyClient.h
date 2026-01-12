#ifndef VARIFYCLIENT_H
#define VARIFYCLIENT_H

#include "const.h"
#include "Singleton.h"
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"

class RPConPool
{
public:
    RPConPool(size_t poolsize, std::string host, std::string port);
    ~RPConPool();
    void Close();
    std::unique_ptr<message::VarifyService::Stub> GetConnection();
    void ReturnConnection(std::unique_ptr<message::VarifyService::Stub> conn);
private:
    size_t poolSize_;
    std::string host_;
    std::string port_;
    std::mutex mutex_;
    std::atomic<bool> b_stop_;
    std::condition_variable cond_;
    std::queue<std::unique_ptr<message::VarifyService::Stub>> connections_;
};

class VarifyClient : public Singleton<VarifyClient>
{
    friend class Singleton<VarifyClient>;

public:
    message::GetVarifyRsp GetVarifyCode(std::string email);
    ~VarifyClient();

private:
    VarifyClient();
    std::unique_ptr<RPConPool> pool_;
};

#endif
