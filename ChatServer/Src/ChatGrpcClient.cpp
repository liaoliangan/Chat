#include "ChatGrpcClient.h"

ChatGrpcClient::ChatGrpcClient()
{
    auto &cfg = ConfigMgr::GetInstance();
    auto server_list = cfg["PeerServer"]["Servers"];

    std::vector<std::string> words;

    std::stringstream ss(server_list);
    std::string word;

    while (std::getline(ss, word, ','))
    {
        words.push_back(word);
    }

    for (auto &word : words)
    {
        if (cfg[word]["Name"].empty())
        {
            continue;
        }
        _pools[cfg[word]["Name"]] = std::make_unique<ChatConPool>(5, cfg[word]["Host"], cfg[word]["Port"]);
    }
}

ChatGrpcClient::~ChatGrpcClient() = default;

message::AddFriendRsp ChatGrpcClient::NotifyAddFriend(std::string server_ip, const message::AddFriendReq &req)
{
    message::AddFriendRsp rsp;
    return rsp;
}

message::AuthFriendRsp ChatGrpcClient::NotifyAuthFriend(std::string server_ip, const message::AuthFriendReq &req)
{
    message::AuthFriendRsp rsp;
    return rsp;
}

bool ChatGrpcClient::GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo> &userinfo)
{
    return true;
}

message::TextChatMsgRsp ChatGrpcClient::NotifyTextChatMsg(std::string server_ip,
                                                          const message::TextChatMsgReq &req, const Json::Value &rtvalue)
{

    message::TextChatMsgRsp rsp;
    return rsp;
}
