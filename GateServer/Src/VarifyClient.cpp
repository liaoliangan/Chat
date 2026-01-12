#include "VarifyClient.h"
#include "ConfigMgr.h"


VarifyClient::VarifyClient()
{
    auto &gCfgMgr = ConfigMgr::GetInstance();
    std::string host = gCfgMgr["VarifyServer"]["Host"];
    std::string port = gCfgMgr["VarifyServer"]["Port"];
    pool_.reset(new RPConPool(5, host, port));
}

message::GetVarifyRsp VarifyClient::GetVarifyCode(std::string email)
{
    grpc::ClientContext context;
    message::GetVarifyRsp reply;   // 回复的响应
    message::GetVarifyReq request; // 请求的参数
    request.set_email(email);
    auto stub = pool_->GetConnection();
    grpc::Status status = stub->GetVarifyCode(&context, request, &reply);
    if (status.ok())
    {
        pool_->ReturnConnection(std::move(stub));
        return reply;
    }
    else
    {
        pool_->ReturnConnection(std::move(stub));
        reply.set_error(static_cast<int>(LA::ErrorCodes::RPCFAILED));
        return reply;
    }
}

VarifyClient::~VarifyClient()
{
}
