#include "StatusServiceImpl.h"
#include "ConfigMgr.h"
#include "const.h"
#include "RedisMgr.h"
#include <climits>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
std::string generate_unique_string() {
	// 创建UUID对象
	boost::uuids::uuid uuid = boost::uuids::random_generator()();

	// 将UUID转换为字符串
	std::string unique_string = boost::uuids::to_string(uuid);

	return unique_string;
}

grpc::Status StatusServiceImpl::GetChatServer(grpc::ServerContext* context, const message::GetChatServerReq* request, message::GetChatServerRsp* reply)
{
	std::string prefix("LA status server has received :  ");
    _server_index = (_server_index++) % (_servers.size());
    auto &server = _servers[_server_index];
    reply->set_host(server.host);
    reply->set_port(server.port);
    reply->set_error(static_cast<int>(LA::ErrorCodes::SUCCESS));
    reply->set_token(generate_unique_string());
    return grpc::Status::OK;
}

StatusServiceImpl::StatusServiceImpl()
{
	 auto& cfg = ConfigMgr::GetInstance();
    ChatServer server;
    server.port = cfg["ChatServer1"]["Port"];
    server.host = cfg["ChatServer1"]["Host"];
    _servers.push_back(server);
    server.port = cfg["ChatServer2"]["Port"];
    server.host = cfg["ChatServer2"]["Host"];
    _servers.push_back(server);

}
grpc::Status StatusServiceImpl::Login(grpc::ServerContext* context, const message::LoginReq* request, message::LoginRsp* reply)
{
	auto uid = request->uid();
	auto token = request->token();

	std::string uid_str = std::to_string(uid);
	std::string token_key = USERTOKENPREFIX + uid_str;
	std::string token_value = "";
	bool success = RedisMgr::GetInstance()->Get(token_key, token_value);
	if (success) {
		reply->set_error(static_cast<int>(LA::ErrorCodes::UID_INVALID));
		return grpc::Status::OK;
	}

	if (token_value != token) {
		reply->set_error(static_cast<int>(LA::ErrorCodes::TOKEN_INVALID));
		return grpc::Status::OK;
	}
	reply->set_error(static_cast<int>(LA::ErrorCodes::SUCCESS));
	reply->set_uid(uid);
	reply->set_token(token);
	return grpc::Status::OK;
}
