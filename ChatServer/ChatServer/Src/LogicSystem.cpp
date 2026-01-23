#include "LogicSystem.h"
#include "MysqlMgr.h"
#include "const.h"
#include "RedisMgr.h"
#include <string>
#include "CServer.h"
#include "UserMgr.h"
#include "StatusGrpcClient.h"
using namespace std;

LogicSystem::LogicSystem() : _b_stop(false), _p_server(nullptr)
{
    RegisterCallBacks();
    _worker_thread = std::thread(&LogicSystem::DealMsg, this);
}

LogicSystem::~LogicSystem()
{
    _b_stop = true;
    _consume.notify_one();
    _worker_thread.join();
}

void LogicSystem::PostMsgToQue(shared_ptr<LogicNode> msg)
{
    std::unique_lock<std::mutex> unique_lk(_mutex);
    _msg_que.push(msg);
    // 由0变为1则发送通知信号
    if (_msg_que.size() == 1)
    {
        unique_lk.unlock();
        _consume.notify_one();
    }
}

void LogicSystem::SetServer(std::shared_ptr<CServer> pserver)
{
    _p_server = pserver;
}

void LogicSystem::DealMsg()
{
    for (;;)
    {
        std::unique_lock<std::mutex> unique_lk(_mutex);
        // 判断队列为空则用条件变量阻塞等待，并释放锁
        while (_msg_que.empty() && !_b_stop)
        {
            _consume.wait(unique_lk);
        }

        // 判断是否为关闭状态，把所有逻辑执行完后则退出循环
        if (_b_stop)
        {
            while (!_msg_que.empty())
            {
                auto msg_node = _msg_que.front();
                cout << "recv_msg id  is " << msg_node->_recvnode->_msg_id << endl;
                auto call_back_iter = _fun_callbacks.find(msg_node->_recvnode->_msg_id);
                if (call_back_iter == _fun_callbacks.end())
                {
                    _msg_que.pop();
                    continue;
                }
                call_back_iter->second(msg_node->_session, msg_node->_recvnode->_msg_id,
                                       std::string(msg_node->_recvnode->_data, msg_node->_recvnode->_cur_len));
                _msg_que.pop();
            }
            break;
        }

        // 如果没有停服，且说明队列中有数据
        auto msg_node = _msg_que.front();
        cout << "recv_msg id  is " << msg_node->_recvnode->_msg_id << endl;
        auto call_back_iter = _fun_callbacks.find(msg_node->_recvnode->_msg_id);
        if (call_back_iter == _fun_callbacks.end())
        {
            _msg_que.pop();
            std::cout << "msg id [" << msg_node->_recvnode->_msg_id << "] handler not found" << std::endl;
            continue;
        }
        std::cout << "msg id [" << msg_node->_recvnode->_msg_id << "] handler found" << std::endl;
        call_back_iter->second(msg_node->_session, msg_node->_recvnode->_msg_id,
                               std::string(msg_node->_recvnode->_data, msg_node->_recvnode->_cur_len));
        _msg_que.pop();
    }
}

void LogicSystem::RegisterCallBacks()
{
    _fun_callbacks[MSG_CHAT_LOGIN] = std::bind(&LogicSystem::LoginHandler, this,
                                               placeholders::_1, placeholders::_2, placeholders::_3);

    _fun_callbacks[ID_SEARCH_USER_REQ] = std::bind(&LogicSystem::SearchInfo, this,
                                                   placeholders::_1, placeholders::_2, placeholders::_3);

    _fun_callbacks[ID_ADD_FRIEND_REQ] = std::bind(&LogicSystem::AddFriendApply, this,
                                                  placeholders::_1, placeholders::_2, placeholders::_3);

    _fun_callbacks[ID_AUTH_FRIEND_REQ] = std::bind(&LogicSystem::AuthFriendApply, this,
                                                   placeholders::_1, placeholders::_2, placeholders::_3);

    _fun_callbacks[ID_TEXT_CHAT_MSG_REQ] = std::bind(&LogicSystem::DealChatTextMsg, this,
                                                     placeholders::_1, placeholders::_2, placeholders::_3);

    _fun_callbacks[ID_HEART_BEAT_REQ] = std::bind(&LogicSystem::HeartBeatHandler, this,
                                                  placeholders::_1, placeholders::_2, placeholders::_3);
}

void LogicSystem::LoginHandler(shared_ptr<CSession> session, const short &msg_id, const string &msg_data)
{
    Json::Reader reader;
    Json::Value root;
    reader.parse(msg_data, root);
    auto uid = root["uid"].asInt();
    auto token = root["token"].asString();
    std::cout << "user login uid is  " << uid << " user token  is "
              << token << endl;

    Json::Value rtvalue;
    Defer defer([this, &rtvalue, session]()
                {
		std::string return_str = rtvalue.toStyledString();
        std::cout << "return_str is " << return_str << std::endl;
		session->Send(return_str, MSG_CHAT_LOGIN_RSP); });

    // 从redis获取用户token是否正确
    std::string uid_str = std::to_string(uid);
    std::string token_key = USERTOKENPREFIX + uid_str;
    std::string token_value = "";

    bool success = RedisMgr::GetInstance()->Get(token_key, token_value);
    if (!success)
    {
        std::cout << "RedisMgr::GetInstance()->Get error" << std::endl;
        rtvalue["error"] = static_cast<int>(LA::ErrorCodes::UID_INVALIED);
        return;
    }

    if (token_value != token)
    {
        std::cout << "token_value != token" << std::endl;
        rtvalue["error"] = static_cast<int>(LA::ErrorCodes::TOKEN_INVALIED);
        return;
    }

    rtvalue["error"] = static_cast<int>(LA::ErrorCodes::SUCCESS);

    std::string base_key = USER_BASE_INFO + uid_str;
    auto user_info = std::make_shared<UserInfo>();
    bool b_base = GetBaseInfo(base_key, uid, user_info);
    if (!b_base)
    {
        rtvalue["error"] = static_cast<int>(LA::ErrorCodes::UID_INVALIED);
        return;
    }
    rtvalue["uid"] = uid;
    rtvalue["pwd"] = user_info->pwd;
    rtvalue["name"] = user_info->name;
    rtvalue["email"] = user_info->email;
    rtvalue["nick"] = user_info->nick;
    rtvalue["desc"] = user_info->desc;
    rtvalue["sex"] = user_info->sex;
    rtvalue["icon"] = user_info->icon;

    // TODO 从数据库获取申请列表

    // TODO 获取好友列表

    auto server_name = ConfigMgr::GetInstance().GetValue("SelfServer", "Name");
    // 将登陆数量增加
    auto rd_res = RedisMgr::GetInstance()->HGet(LOGIN_COUNT, server_name);
    int count = 0;
    if (!rd_res.empty())
    {
        count = std::stoi(rd_res);
    }
    count++;

    auto count_str = std::to_string(count);
    RedisMgr::GetInstance()->HSet(LOGIN_COUNT, server_name, count_str);

    // session绑定用户Uid
    session->SetUserId(uid);

    // 为用户设置登录ip server的名字
    std::string ipkey = USERIPPREFIX + uid_str;
    RedisMgr::GetInstance()->Set(ipkey, server_name);
    // uid和session绑定管理,方便以后踢人操作
    UserMgr::GetInstance()->SetUserSession(uid, session);
}

void LogicSystem::SearchInfo(std::shared_ptr<CSession> session, const short &msg_id, const string &msg_data)
{
}

void LogicSystem::AddFriendApply(std::shared_ptr<CSession> session, const short &msg_id, const string &msg_data)
{
}

void LogicSystem::AuthFriendApply(std::shared_ptr<CSession> session, const short &msg_id, const string &msg_data)
{
}

void LogicSystem::DealChatTextMsg(std::shared_ptr<CSession> session, const short &msg_id, const string &msg_data)
{
}

void LogicSystem::HeartBeatHandler(std::shared_ptr<CSession> session, const short &msg_id, const string &msg_data)
{
}

bool LogicSystem::isPureDigit(const std::string &str)
{
    return false;
}

void LogicSystem::GetUserByUid(std::string uid_str, Json::Value &rtvalue)
{
}

void LogicSystem::GetUserByName(std::string name, Json::Value &rtvalue)
{
}

bool LogicSystem::GetFriendApplyInfo(int to_uid, std::vector<std::shared_ptr<ApplyInfo>> &list)
{
    return false;
}
bool LogicSystem::GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo> &userinfo)
{
    std::cout << "GetBaseInfo: base_key is " << base_key << " uid is " << uid << " userinfo is " << *(userinfo.get()) << std::endl;
    // 优先查redis中查询用户信息
    std::string info_str = "";
    bool b_base = RedisMgr::GetInstance()->Get(base_key, info_str);
    if (b_base)
    {
        std::cout << "GetBaseInfo: redis_info_str is " << info_str << std::endl;
        Json::Reader reader;
        Json::Value root;
        reader.parse(info_str, root);
        userinfo->uid = root["uid"].asInt();
        userinfo->name = root["name"].asString();
        userinfo->pwd = root["pwd"].asString();
        userinfo->email = root["email"].asString();
        userinfo->nick = root["nick"].asString();
        userinfo->desc = root["desc"].asString();
        userinfo->sex = root["sex"].asInt();
        userinfo->icon = root["icon"].asString();
        std::cout << "user login uid is  " << userinfo->uid << " name  is "
                  << userinfo->name << " pwd is " << userinfo->pwd << " email is " << userinfo->email << endl;
    }
    else
    {
        std::cout << "GetBaseInfo: redis_info_str is null" << std::endl;
        // redis中没有则查询mysql
        // 查询数据库
        std::shared_ptr<UserInfo> user_info = nullptr;
        user_info = MysqlMgr::GetInstance()->GetUser(uid);
        if (user_info == nullptr)
        {
            return false;
        }

        userinfo = user_info;

        // 将数据库内容写入redis缓存
        Json::Value redis_root;
        redis_root["uid"] = uid;
        redis_root["pwd"] = userinfo->pwd;
        redis_root["name"] = userinfo->name;
        redis_root["email"] = userinfo->email;
        // redis_root["nick"] = userinfo->nick;
        // redis_root["desc"] = userinfo->desc;
        // redis_root["sex"] = userinfo->sex;
        // redis_root["icon"] = userinfo->icon;
        RedisMgr::GetInstance()->Set(base_key, redis_root.toStyledString());
    }

    return true;
}

bool LogicSystem::GetFriendList(int self_id, std::vector<std::shared_ptr<UserInfo>> &user_list)
{
    return false;
}
