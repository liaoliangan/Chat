#ifndef CHAT_GRPC_CLIENT_H
#define CHAT_GRPC_CLIENT_H

#include "const.h"
#include "Singleton.h"
#include "ConfigMgr.h"
#include "message.grpc.pb.h"
#include "message.pb.h"
#include <queue>
#include <unordered_map>
#include "data.h"
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <grpcpp/grpcpp.h>

class ChatConPool
{
public:
    ChatConPool(size_t poolSize, std::string host, std::string port) : poolSize_(poolSize), host_(host), port_(port), b_stop_(false)
    {
        for (size_t i = 0; i < poolSize_; ++i)
        {
            std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(host + ":" + port, grpc::InsecureChannelCredentials());
            connections_.push(message::ChatService::NewStub(channel));
        }
    }

    ~ChatConPool()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        Close();
        while (!connections_.empty())
        {
            connections_.pop();
        }
    }

    std::unique_ptr<message::ChatService::Stub> getConnection()
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

    void returnConnection(std::unique_ptr<message::ChatService::Stub> context)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (b_stop_)
        {
            return;
        }
        connections_.push(std::move(context));
        cond_.notify_one();
    }

    void Close()
    {
        b_stop_ = true;
        cond_.notify_all();
    }

private:
    std::atomic<bool> b_stop_;
    size_t poolSize_;
    std::string host_;
    std::string port_;
    std::queue<std::unique_ptr<message::ChatService::Stub>> connections_;
    std::mutex mutex_;
    std::condition_variable cond_;
};

class ChatGrpcClient : public Singleton<ChatGrpcClient>
{
    friend class Singleton<ChatGrpcClient>;

public:
    ~ChatGrpcClient();
    message::AddFriendRsp NotifyAddFriend(std::string server_ip, const message::AddFriendReq &req);
    message::AuthFriendRsp NotifyAuthFriend(std::string server_ip, const message::AuthFriendReq &req);
    bool GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo> &userinfo);
    message::TextChatMsgRsp NotifyTextChatMsg(std::string server_ip, const message::TextChatMsgReq &req, const Json::Value &rtvalue);
    message::KickUserRsp NotifyKickUser(std::string server_ip, const message::KickUserReq &req);

private:
    ChatGrpcClient();
    std::unordered_map<std::string, std::unique_ptr<ChatConPool>> _pools;
};

#endif
