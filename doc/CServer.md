## CServer

```cpp
class CServer : public std::enable_shared_from_this<CServer>
{
public:
    CServer(boost::asio::io_context& io_context, short port);
	~CServer();
	void ClearSession(std::string);
	//uidȡsession
	shared_ptr<CSession> GetSession(std::string);
	bool CheckValid(std::string);
	void on_timer(const boost::system::error_code& ec);
	void StartTimer();
	void StopTimer();
private:
	void HandleAccept(shared_ptr<CSession>, const boost::system::error_code & error);
	void StartAccept();
	boost::asio::io_context &_io_context;
	short _port;
	tcp::acceptor _acceptor;
	std::map<std::string, shared_ptr<CSession>> _sessions;
	std::mutex _mutex;
	boost::asio::steady_timer _timer;
};
```

`boost::asio::ip::tcp::acceptor` 接收器，用来接受一个连接

```cpp
void CServer::StartAccept()
{
    auto &io_context = AsioIOContextPool::GetInstance()->GetIOContext();
    shared_ptr<CSession> new_session = make_shared<CSession>(io_context, this);
    _acceptor.async_accept(new_session->GetSocket(), std::bind(&CServer::HandleAccept, this, new_session, placeholders::_1));
}
```

异步接受连接，每一个连接开一个Session，每一个Session和用户的Uid双向绑定，可以通过uid查找到Session

## Session

`异步读数据头`

```cpp
void CSession::AsyncReadHead(int total_len)
{
    auto self = shared_from_this();
    asyncReadFull(HEAD_TOTAL_LEN, [self, this](const boost::system::error_code &ec, std::size_t bytes_transfered)
                  {
		try {
			if (ec) {
				std::cout << "handle read failed, error is " << ec.what() << endl;
				Close();
				DealExceptionSession();
				return;
			}

			if (bytes_transfered < HEAD_TOTAL_LEN) {
				std::cout << "read length not match, read [" << bytes_transfered << "] , total ["
					<< HEAD_TOTAL_LEN << "]" << endl;
				Close();
				_server->ClearSession(_session_id);
				return;
			}

			//判断连接无效
			if (!_server->CheckValid(_session_id)) {
				Close();
				return;
			}

			_recv_head_node->Clear();
			memcpy(_recv_head_node->_data, _data, bytes_transfered);

			//获取头部MSGID数据
			short msg_id = 0;
			memcpy(&msg_id, _recv_head_node->_data, HEAD_ID_LEN);
			//网络字节序转化为本地字节序
			msg_id = boost::asio::detail::socket_ops::network_to_host_short(msg_id);
			std::cout << "msg_id is " << msg_id << endl;
			//id非法
			if (msg_id > MAX_LENGTH) {
				std::cout << "invalid msg_id is " << msg_id << endl;
				_server->ClearSession(_session_id);
				return;
			}
			short msg_len = 0;
			memcpy(&msg_len, _recv_head_node->_data + HEAD_ID_LEN, HEAD_DATA_LEN);
			//网络字节序转化为本地字节序
			msg_len = boost::asio::detail::socket_ops::network_to_host_short(msg_len);
			std::cout << "msg_len is " << msg_len << endl;

			//id非法
			if (msg_len > MAX_LENGTH) {
				std::cout << "invalid data length is " << msg_len << endl;
				_server->ClearSession(_session_id);
				return;
			}

			_recv_msg_node = make_shared<RecvNode>(msg_len, msg_id);
			AsyncReadBody(msg_len);
		}
		catch (std::exception& e) {
			std::cout << "Exception code is " << e.what() << endl;
		} });
}
```

如果读取到的数据长度<数据头的长度，放弃此次读取，直到读取的长度>=数据头的长度

```cpp
if (bytes_transfered < HEAD_TOTAL_LEN) {
		std::cout << "read length not match, read [" << bytes_transfered << "] , total [" << HEAD_TOTAL_LEN << "]" << endl;
		Close();
		_server->ClearSession(_session_id);
		return;
}
```

读入到的长度>=数据头

```cpp
memcpy(_recv_head_node->_data, _data, bytes_transfered);
//获取头部MSGID数据
short msg_id = 0;
memcpy(&msg_id, _recv_head_node->_data, HEAD_ID_LEN);
//网络字节序转化为本地字节序
msg_id = boost::asio::detail::socket_ops::network_to_host_short(msg_id);
std::cout << "msg_id is " << msg_id << endl;
//id非法
if (msg_id > MAX_LENGTH) {
	std::cout << "invalid msg_id is " << msg_id << endl;
	_server->ClearSession(_session_id);
	return;
}
short msg_len = 0;
memcpy(&msg_len, _recv_head_node->_data + HEAD_ID_LEN, HEAD_DATA_LEN);
//网络字节序转化为本地字节序
msg_len = boost::asio::detail::socket_ops::network_to_host_short(msg_len);
std::cout << "msg_len is " << msg_len << endl;
//id非法
if (msg_len > MAX_LENGTH) {
std::cout << "invalid data length is " << msg_len << endl;
_server->ClearSession(_session_id);
	return;
}
_recv_msg_node = make_shared<RecvNode>(msg_len, msg_id);
AsyncReadBody(msg_len);
```

`异步读数据`

```cpp
void CSession::AsyncReadBody(int total_len)
{
    auto self = shared_from_this();
    asyncReadFull(total_len, [self, this, total_len](const boost::system::error_code &ec, std::size_t bytes_transfered)
                  {
		try {
			if (ec) {
				std::cout << "handle read failed, error is " << ec.what() << endl;
				Close();
				DealExceptionSession();
				return;
			}
			if (bytes_transfered < total_len) {
				std::cout << "read length not match, read [" << bytes_transfered << "] , total ["
					<< total_len<<"]" << endl;
				Close();
				_server->ClearSession(_session_id);
				return;
			}

			//判断连接无效
			if (!_server->CheckValid(_session_id)) {
				Close();
				return;
			}

			memcpy(_recv_msg_node->_data , _data , bytes_transfered);
			_recv_msg_node->_cur_len += bytes_transfered;
			_recv_msg_node->_data[_recv_msg_node->_total_len] = '\0';
			cout << "receive data is " << _recv_msg_node->_data << endl;
			//更新session心跳时间
			UpdateHeartbeat();
			//此处将消息投递到逻辑队列中
			LogicSystem::GetInstance()->PostMsgToQue(make_shared<LogicNode>(shared_from_this(), _recv_msg_node));
			//继续监听头部接受事件
			AsyncReadHead(HEAD_TOTAL_LEN);
		}
		catch (std::exception& e) {
			std::cout << "Exception code is " << e.what() << endl;
		} });
}
```

`LogicSystem`存在一个消息队列，所有的消息都被投放在消息队列

当消息队列中存在数据时，通过信号量通知其他线程进行消息处理`生产者消费者模型`

```cpp
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
```



`LogicSystem`中存在一个线程用来处理消息队列中的消息

```cpp
_worker_thread = std::thread(&LogicSystem::DealMsg, this);
```

```cpp
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
        std::cout << "msg id [" << msg_node->_recvnode->_msg_id << "] handler done" << std::endl;
        _msg_que.pop();
    }
}
```

在消息处理线程中会根据消息的`ID`执行对应的回调函数

```cp
call_back_iter->second(msg_node->_session, msg_node->_recvnode->_msg_id,
                               std::string(msg_node->_recvnode->_data, msg_node->_recvnode->_cur_len));
```

