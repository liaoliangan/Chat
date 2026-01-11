#include "const.h"

class HttpConnection : public std::enable_shared_from_this<HttpConnection>
{
    friend class LogicSystem;

public:
    HttpConnection(boost::asio::ip::tcp::socket socket);
    void Start();

private:
    void CheckDeadline();
    void WriteResponse();
    void HandleReq();
    void PreParseGetParam();
    boost::asio::ip::tcp::socket _socket;
    // The buffer for performing reads.
    boost::beast::flat_buffer _buffer{8192};

    // The request message.
    boost::beast::http::request<boost::beast::http::dynamic_body> _request;

    // The response message.
    boost::beast::http::response<boost::beast::http::dynamic_body> _response;

    // The timer for putting a deadline on connection processing.
    boost::asio::steady_timer deadline_{
        _socket.get_executor(), std::chrono::seconds(60)};

    std::string _get_url;
    std::unordered_map<std::string, std::string> _get_params;
};
