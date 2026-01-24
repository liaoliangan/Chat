#ifndef CHATSERVICEIMPL_H
#define CHATSERVICEIMPL_H

#include <grpc/grpc.h>
#include "message.grpc.pb.h"
#include "data.h"
#include <mutex>
#include "CServer.h"
class ChatServiceImpl final : public message::ChatService::Service
{
public:
    ChatServiceImpl();
    grpc::Status NotifyAddFriend(::grpc::ServerContext *context, const message::AddFriendReq *request,
                                 message::AddFriendRsp *reply) override;

    grpc::Status NotifyAuthFriend(::grpc::ServerContext *context,
                                  const message::AuthFriendReq *request, message::AuthFriendRsp *response) override;

    grpc::Status NotifyTextChatMsg(::grpc::ServerContext *context,
                                   const message::TextChatMsgReq *request, message::TextChatMsgRsp *response) override;

    bool GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo> &userinfo);

    // 接受rpc踢人请求
    grpc::Status NotifyKickUser(::grpc::ServerContext *context,
                                const message::KickUserReq *request, message::KickUserRsp *response) override;

    void RegisterServer(std::shared_ptr<CServer> pServer);

private:
    std::shared_ptr<CServer> _p_server;
};

#endif
