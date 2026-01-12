cmake_minimum_required(VERSION 3.10)
project(GateServer)

# 设置 C++11 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BOOST_ROOT /usr/local) # 指向你安装的 1.78
set(Boost_NO_SYSTEM_PATHS ON) # 不再扫 /usr/lib/x86_64-linux-gnu
set(CMAKE_NO_SYSTEM_FROM_IMPORTED ON) # 禁止任何目标塞 -isystem /usr/include

# 正确写法
file(GLOB SRCFILE
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/*.cc)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Inc)


# 假设已经安装好grpc了
find_package(Threads REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(Python3 REQUIRED COMPONENTS Development)

message(STATUS "Using gRPC ${gRPC_VERSION}")
message(STATUS "Using protobuf ${Protobuf_VERSION}")

add_executable(${CMAKE_PROJECT_NAME}
    ${SRCFILE}
    main.cpp
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${Python3_INCLUDE_DIRS})

target_link_libraries(${CMAKE_PROJECT_NAME}
    jsoncpp
    gRPC::grpc++_reflection
    gRPC::grpc++
    protobuf::libprotobuf
    Python3::Python
)
