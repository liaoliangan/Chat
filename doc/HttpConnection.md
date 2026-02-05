## HttpConnection

客户端在登录时，与`GateServer`进行的是`http`连接而不是`tcp`，只有登陆完成客户端连接上聊天服务器后才进行的`tcp`长连接

在`HttpConnection`中异步的读取http请求

```cpp
void HttpConnection::Start()
{
    auto self = shared_from_this();
    boost::beast::http::async_read(_socket, _buffer, _request, [self](boost::beast::error_code ec, std::size_t bytes_transferred)
                                   {
            try {
                if (ec) {
                    std::cout << "http read err is " << ec.what() << std::endl;
                    return;
                }

                //处理读到的数据
                boost::ignore_unused(bytes_transferred);
                self->HandleReq();
                self->CheckDeadline();
            }
            catch (std::exception& exp) {
                std::cout << "exception is " << exp.what() << std::endl;
            } });
}
```

请求的处理函数

```cpp
void HttpConnection::HandleReq()
{
    // 设置版本
    _response.version(_request.version());
    // 设置为短链接
    _response.keep_alive(false);

    std::cout << "method is " << _request.method() << "\t" << "url is " << _request.target() << std::endl;
    if (_request.method() == boost::beast::http::verb::get)
    {
        PreParseGetParam();
        bool success = LogicSystem::GetInstance()->HandleGet(_get_url, shared_from_this());
        if (!success)
        {
            _response.result(boost::beast::http::status::not_found);
            _response.set(boost::beast::http::field::content_type, "text/plain");
            boost::beast::ostream(_response.body()) << "url not found\r\n";
            WriteResponse();
            return;
        }

        _response.result(boost::beast::http::status::ok);
        _response.set(boost::beast::http::field::server, "GateServer");
        WriteResponse();
        return;
    }
    else if (_request.method() == boost::beast::http::verb::post)
    {
        bool success = LogicSystem::GetInstance()->HandlePost(std::string(_request.target()), shared_from_this());
        if (!success)
        {
            _response.result(boost::beast::http::status::not_found);
            _response.set(boost::beast::http::field::content_type, "text/plain");
            boost::beast::ostream(_response.body()) << "url not found\r\n";
            WriteResponse();
            return;
        }

        _response.result(boost::beast::http::status::ok);
        _response.set(boost::beast::http::field::server, "GateServer");
        WriteResponse();
        return;
    }
}
```

如果是`get`请求，需要进行查询参数的解析

```cpp
void HttpConnection::PreParseGetParam()
{
    // 提取 URI
    boost::beast::string_view uri = _request.target();
    // 查找查询字符串的开始位置（即 '?' 的位置）
    std::size_t query_pos = uri.find('?');
    if (query_pos == std::string_view::npos)
    {
        _get_url = std::string(uri);
        return;
    }

    _get_url = std::string(uri.substr(0, query_pos));
    std::string query_string = std::string(uri.substr(query_pos + 1));
    std::string key;
    std::string value;
    size_t pos = 0;
    while ((pos = query_string.find('&')) != std::string::npos)
    {
        auto pair = query_string.substr(0, pos);
        size_t eq_pos = pair.find('=');
        if (eq_pos != std::string::npos)
        {
            key = UrlDecode(pair.substr(0, eq_pos)); // 假设有 url_decode 函数来处理URL解码
            value = UrlDecode(pair.substr(eq_pos + 1));
            _get_params[key] = value;
        }
        query_string.erase(0, pos + 1);
    }
    // 处理最后一个参数对（如果没有 & 分隔符）
    if (!query_string.empty())
    {
        size_t eq_pos = query_string.find('=');
        if (eq_pos != std::string::npos)
        {
            key = UrlDecode(query_string.substr(0, eq_pos));
            value = UrlDecode(query_string.substr(eq_pos + 1));
            _get_params[key] = value;
        }
    }
}
```

`GateServer`中也有`LogicSystem`用来处理http的数据，其中有两个map，根据`url`来存储回调函数

```cpp
std::map<std::string, HttpHandler> _post_handlers;
std::map<std::string, HttpHandler> _get_handlers;
```

最终执行完回调函数后，统一进行请求的响应

```cpp
void HttpConnection::WriteResponse()
{
    auto self = shared_from_this();
    _response.content_length(_response.body().size());
    boost::beast::http::async_write(
        _socket,
        _response,
        [self](boost::beast::error_code ec, std::size_t)
        {
            self->_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
            self->deadline_.cancel();
        });
}
```

