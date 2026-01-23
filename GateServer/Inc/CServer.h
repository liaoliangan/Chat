#ifndef CSERVER_H
#define CSERVER_H

#include "const.h"

class CServer : public std::enable_shared_from_this<CServer>
{
public:
    CServer(boost::asio::io_context &ioc, unsigned short port);
    void Start();

private:
    boost::asio::ip::tcp::acceptor _acceptor;
    boost::asio::io_context &_ioc;
    // boost::asio::ip::tcp::socket _socket;
};

#endif