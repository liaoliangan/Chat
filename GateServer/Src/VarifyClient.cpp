#include "VarifyClient.h"

VarifyClient::VarifyClient()
{
    std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials());
    stub = message::VarifyService::NewStub(channel);
}

message::GetVarifyRsp VarifyClient::GetVarifyCode(std::string email)
{
    grpc::ClientContext context;
    message::GetVarifyRsp reply;   // 回复的响应
    message::GetVarifyReq request; // 请求的参数
    request.set_email(email);

    grpc::Status status = stub->GetVarifyCode(&context, request, &reply);
    if (status.ok())
    {
        return reply;
    }
    else
    {
        reply.set_error(static_cast<int>(LA::ErrorCodes::RPCFAILED));
        return reply;
    }
}

VarifyClient::~VarifyClient()
{
}
