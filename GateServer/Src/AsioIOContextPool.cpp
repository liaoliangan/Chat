#include "AsioIOContextPool.h"
#include <iostream>
using namespace std;
AsioIOContextPool::AsioIOContextPool(std::size_t size):_ioContexts(size),
_works(size), _nextIOContext(0){
    for (std::size_t i = 0; i < size; ++i) {
        _works[i] = std::unique_ptr<boost::asio::io_context::work>(new boost::asio::io_context::work(_ioContexts[i]));
    }

    //遍历多个ioservice，创建多个线程，每个线程内部启动ioservice
    for (std::size_t i = 0; i < _ioContexts.size(); ++i) {
        _threads.emplace_back([this, i]() {
            _ioContexts[i].run();
            });
    }
}

AsioIOContextPool::~AsioIOContextPool() {
    Stop();
    std::cout << "AsioIOServicePool destruct" << endl;
}

boost::asio::io_context& AsioIOContextPool::GetIOContext() {
    auto& context = _ioContexts[_nextIOContext++];
    if (_nextIOContext == _ioContexts.size()) {
        _nextIOContext = 0;
    }
    return context;
}

void AsioIOContextPool::Stop(){
    //因为仅仅执行work.reset并不能让iocontext从run的状态中退出
    //当iocontext已经绑定了读或写的监听事件后，还需要手动stop该服务。
    for (auto& work : _works) {
        //把服务先停止
        work->get_io_context().stop();
        work.reset();
    }

    for (auto& t : _threads) {
        t.join();
    }
}
