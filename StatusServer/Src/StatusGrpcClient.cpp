#include "StatusGrpcClient.h"
#include "Defer"
StatusConPool::StatusConPool(size_t poolsize, std::string host, std::string port) : poolSize_(poolsize), host_(host), port_(port), b_stop_(false)
{
    for (size_t i = 0; i < poolSize_; i++)
    {
        std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(host + ":" + port, grpc::InsecureChannelCredentials());
        connections_.push(message::StatusService::NewStub(channel));
    }
}

StatusConPool::~StatusConPool()
{
    std::lock_guard<std::mutex> lock(mutex_);
    Close();
    while (!connections_.empty())
    {
        connections_.pop();
    }
}

void StatusConPool::Close()
{
    b_stop_ = true;
    cond_.notify_all();
}

std::unique_ptr<message::StatusService::Stub> StatusConPool::GetConnection()
{
    std::unique_lock<std::mutex> lock(mutex_);
    cond_.wait(lock, [this]
               {
            if (b_stop_) {
                return true;
            }
            return !connections_.empty(); });
    // 如果停止则直接返回空指针
    if (b_stop_)
    {
        return nullptr;
    }
    auto context = std::move(connections_.front());
    connections_.pop();
    return context;
}

void StatusConPool::ReturnConnection(std::unique_ptr<message::StatusService::Stub> conn)
{
    std::lock_guard<std::mutex> lock(mutex_);
    connections_.push(std::move(conn));
    cond_.notify_one();
}
StatusGrpcClient::StatusGrpcClient()
{
    auto &gCfgMgr = ConfigMgr::GetInstance();
    std::string host = gCfgMgr["StatusServer"]["Host"];
    std::string port = gCfgMgr["StatusServer"]["Port"];
    pool_.reset(new StatusConPool(5, host, port));
}
StatusGrpcClient::~StatusGrpcClient() = default;

message::GetChatServerRsp StatusGrpcClient::GetChatServer(int uid)
{
    grpc::ClientContext context;
    message::GetChatServerRsp reply;
    message::GetChatServerReq request;
    request.set_uid(uid);
    auto stub = pool_->GetConnection();
    grpc::Status status = stub->GetChatServer(&context, request, &reply);
    Defer defer([&stub, this]()
                { pool_->ReturnConnection(std::move(stub)); });
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
