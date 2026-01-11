cmake_minimum_required(VERSION 3.10)
project(GateServer)

file(GLOB SRCFILE ${CMAKE_CURRENT_SOURCE_DIR}/Src/*.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Inc)

add_executable(${CMAKE_PROJECT_NAME}
    ${SRCFILE}
    main.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    jsoncpp
)
