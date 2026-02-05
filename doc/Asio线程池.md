## BOOST.ASIO线程池

以轮询的方式获得io_context，每一个io_context跑在不同的线程，实现并行

```cpp
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
    AsioIOContextPool(std::size_t size = std::thread::hardware_concurrency());
    std::vector<boost::asio::io_context> _ioContexts;
    std::vector<std::unique_ptr<boost::asio::io_context::work>> _works;
    std::vector<std::thread> _threads;
    std::size_t                        _nextIOContext;
};
```

`boost::asio::io_context::work` io_context的工作现场，每一个上下文的运行在一个work中，防止没有任何连接使用`io_context`使其析构

```cpp
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
```

