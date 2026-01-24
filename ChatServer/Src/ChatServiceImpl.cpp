#include "ChatServiceImpl.h"

ChatServiceImpl::ChatServiceImpl()
{
}

grpc::Status ChatServiceImpl::NotifyAddFriend(::grpc::ServerContext *context,
                                                const message::AddFriendReq *request,
                                                message::AddFriendRsp *reply)
{
    return grpc::Status::OK;
}

grpc::Status ChatServiceImpl::NotifyAuthFriend(::grpc::ServerContext *context,
                                                 const message::AuthFriendReq *request,
                                                 message::AuthFriendRsp *response)
{
    return grpc::Status::OK;
}

grpc::Status ChatServiceImpl::NotifyTextChatMsg(::grpc::ServerContext *context,
                                                  const message::TextChatMsgReq *request,
                                                  message::TextChatMsgRsp *response)
{
    return grpc::Status::OK;
}

bool ChatServiceImpl::GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo> &userinfo)
{
    return true;
}
