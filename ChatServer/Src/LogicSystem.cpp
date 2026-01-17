#include "LogicSystem.h"
#include "MysqlMgr.h"
#include "const.h"
#include "RedisMgr.h"
#include <string>
#include "CServer.h"
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
    return false;
}

bool LogicSystem::GetFriendList(int self_id, std::vector<std::shared_ptr<UserInfo>> &user_list)
{
    return false;
}
