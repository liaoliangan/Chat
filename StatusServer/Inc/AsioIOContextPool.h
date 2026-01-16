#ifndef ASIO_IOSERVICE_POOL_H
#define ASIO_IOSERVICE_POOL_H

#include <vector>
#include <boost/asio.hpp>
#include "Singleton.h"
class AsioIOContextPool:public Singleton<AsioIOContextPool>
{
    friend Singleton<AsioIOContextPool>;
public:
    ~AsioIOContextPool();
    AsioIOContextPool(const AsioIOContextPool&) = delete;
    AsioIOContextPool& operator=(const AsioIOContextPool&) = delete;
    // 使用 round-robin 的方式返回一个 io_service
    boost::asio::io_context& GetIOContext();
    void Stop();
private:
    AsioIOContextPool(std::size_t size = 2/*std::thread::hardware_concurrency()*/);
    std::vector<boost::asio::io_context> _ioContexts;
    std::vector<std::unique_ptr<boost::asio::io_context::work>> _works;
    std::vector<std::thread> _threads;
    std::size_t                        _nextIOContext;
};

#endif
