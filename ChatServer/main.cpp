#include "LogicSystem.h"
#include <csignal>
#include <thread>
#include <mutex>
#include "Defer"
#include "AsioIOContextPool.h"
#include "ChatServiceImpl.h"
#include <grpcpp/grpcpp.h>
#include "CServer.h"
#include "RedisMgr.h"
#include "ConfigMgr.h"
#include <boost/asio.hpp>
#include <boost/asio/signal_set.hpp>

bool bstop = false;
std::condition_variable cond_quit;
std::mutex mutex_quit;
int main()
{
    try
    {
        auto &cfg = ConfigMgr::GetInstance();
        auto pool = AsioIOContextPool::GetInstance();
        auto server_name = cfg["SelfServer"]["Name"];
        RedisMgr::GetInstance()->HSet(LOGIN_COUNT, server_name, "0");

        Defer derfer([server_name]()
                     {
				RedisMgr::GetInstance()->HDel(LOGIN_COUNT, server_name);
				RedisMgr::GetInstance()->Close(); });

        std::string server_address(cfg["SelfServer"]["Host"] + ":" + cfg["SelfServer"]["RPCPort"]);
        ChatServiceImpl service;
        grpc::ServerBuilder builder;
        // 监听端口和添加服务
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service);
        // service.RegisterServer(pointer_server);
        // 构建并启动gRPC服务器
        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        std::cout << "RPC Server listening on " << server_address << std::endl;

        // 单独启动一个线程处理grpc服务
        std::thread grpc_server_thread([&server]()
                                       { server->Wait(); });

        boost::asio::io_context ioc{1};
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc, pool, &server](auto, auto)
                           {
            ioc.stop();
            pool->Stop();
            server->Shutdown(); });
        auto port_str = cfg["SelfServer"]["Port"];
        unsigned short port = static_cast<unsigned short>(std::stoi(port_str));
        CServer s(ioc, port);
        ioc.run();
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}
