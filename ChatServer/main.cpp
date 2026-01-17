#include "LogicSystem.h"
#include <csignal>
#include <thread>
#include <mutex>
#include "AsioIOContextPool.h"
#include "CServer.h"
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
        boost::asio::io_context ioc{1};
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc, pool](auto, auto)
                           {
            ioc.stop();
            pool->Stop(); });
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
