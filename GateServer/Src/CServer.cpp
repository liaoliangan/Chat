#include "CServer.h"
#include "AsioIOContextPool.h"
#include "HttpConnection.h"
CServer::CServer(boost::asio::io_context &ioc, unsigned short port) : _ioc(ioc), _acceptor(ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
    // 确保套接字可以重用地址
    _acceptor.set_option(boost::asio::socket_base::reuse_address(true));
}
void CServer::Start()
{
    std::cout << "Starting server..." << std::endl;
    auto self = shared_from_this();
    auto &io_context = AsioIOContextPool::GetInstance()->GetIOContext();
    std::shared_ptr<HttpConnection> new_con = std::make_shared<HttpConnection>(io_context);
    std::cout << "new_con" << std::endl;
    _acceptor.async_accept(new_con->GetSocket(), [self,new_con](boost::beast::error_code ec)
                           {
        try {
            //出错则放弃这个连接，继续监听新链接
            if (ec) {
                std::cout << "Accept error: " << ec.message() << std::endl;
                self->Start();
                return;
            }
            std::cout << "Accept success" << std::endl;
            //处理新链接，创建HpptConnection类管理新连接
            new_con->Start();
            //继续监听
            self->Start();
        }
        catch (std::exception& exp) {
            std::cout << "exception is " << exp.what() << std::endl;
            self->Start();
        } });
}
