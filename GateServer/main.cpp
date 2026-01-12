#include "const.h"
#include <CServer.h>
#include "ConfigMgr.h"
int main()
{
    std::string gate_port = ConfigMgr::GetInstance()["GateServer"]["Port"];
    try
    {
        unsigned short port = static_cast<unsigned short>(atoi(gate_port.c_str()));
        boost::asio::io_context ioc{1};
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc](const boost::system::error_code &error, int signal_number)
                           {

            if (error) {
                return;
            }
            ioc.stop(); });
        std::make_shared<CServer>(ioc, port)->Start();
        ioc.run();
    }
    catch (std::exception const &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
