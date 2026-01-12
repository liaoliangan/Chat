#include "RPConPool.h"

RPConPool::RPConPool(size_t poolsize, std::string host, std::string port) : poolSize_(poolsize), host_(host), port_(port), b_stop_(false)
{
    for (size_t i = 0; i < poolSize_; i++)
    {
        std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(host + ":" + port, grpc::InsecureChannelCredentials());
        connections_.push(message::VarifyService::NewStub(channel));
    }
}

RPConPool::~RPConPool()
{
    std::lock_guard<std::mutex> lock(mutex_);
    Close();
    while (!connections_.empty())
    {
        connections_.pop();
    }
}

void RPConPool::Close()
{
    b_stop_ = true;
    cond_.notify_all();
}

std::unique_ptr<message::VarifyService::Stub> RPConPool::GetConnection()
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

void RPConPool::ReturnConnection(std::unique_ptr<message::VarifyService::Stub> conn)
{
    std::lock_guard<std::mutex> lock(mutex_);
    connections_.push(std::move(conn));
    cond_.notify_one();
}
