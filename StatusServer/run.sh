#!/bin/bash

function run()
{
    cd /home/liao/code/chat/StatusServer/build

    cmake ../ -G "Ninja"

    ninja
}

function start_server()
{
    cd /home/liao/code/chat/StatusServer/build

    # 启动服务器并在后台运行
    ./StatusServer &
    SERVER_PID=$!

    # 保存PID到文件以便后续管理
    echo $SERVER_PID > server.pid

    echo "Server started with PID: $SERVER_PID"

    # 等待几秒让服务器初始化
    sleep 2

    # 检查进程是否还在运行
    if kill -0 $SERVER_PID 2>/dev/null; then
        echo "✓ Server is running"

        # 检查50052端口是否在监听
        if ss -tuln | grep ':50052' > /dev/null; then
            echo "✓ Port 50052 is listening"

            # 尝试发送测试请求
            if curl -s http://localhost:50052/get_test > /dev/null; then
                echo "✓ Server responded to test request"
                echo "✓ Server startup successful!"
            else
                echo "! Server not responding to requests"
            fi
        else
            echo "! Port 8080 is not listening - server may have crashed"
        fi
    else
        echo "✗ Server failed to start"
    fi
}

function stop_server()
{
    if [ -f server.pid ]; then
        SERVER_PID=$(cat server.pid)
        if kill -0 $SERVER_PID 2>/dev/null; then
            kill $SERVER_PID
            echo "Server stopped (PID: $SERVER_PID)"
        else
            echo "Server is not running"
        fi
        rm server.pid
    else
        echo "No server.pid file found"
        # 尝试查找可能的StatusServer进程
        PIDS=$(pgrep StatusServer)
        if [ ! -z "$PIDS" ]; then
            kill $PIDS
            echo "Killed StatusServer process(es): $PIDS"
        fi
    fi
}

function clean()
{
    BUILD_PATH=/home/liao/code/chat/StatusServer/build
    if [ -d "$BUILD_PATH" ]; then
        cd $BUILD_PATH
        rm -rf *
    else
        echo "Error: Directory $BUILD_PATH does not exist."
        exit 1
    fi
}

function rebuild()
{
    clean
    run
}

if [ "$1" == "clean" ]; then
    clean
elif [ "$1" == "rebuild" ]; then
    rebuild
elif [ "$1" == "start" ]; then
    start_server
elif [ "$1" == "stop" ]; then
    stop_server
else
    echo "Usage: $0 [clean|run|rebuild|start|stop]"
    echo "  start   - Build and start the server with status checks"
    echo "  stop    - Stop the running server"
    echo "  clean   - Clean build directory"
    echo "  rebuild - Clean and rebuild the project"
    exit 1
fi
