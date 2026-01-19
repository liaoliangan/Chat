cmake_minimum_required(VERSION 3.30)
project(Client)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "D:/QT/6.5.3/mingw_64/bin")
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/Form)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Network
        SerialPort
        Svg
        REQUIRED)

qt_standard_project_setup()
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cmake-build-debug/Client_autogen/include)

file(GLOB SRCFILE ${CMAKE_CURRENT_SOURCE_DIR}/Src/*.cpp)
file(GLOB UIFILE ${CMAKE_CURRENT_SOURCE_DIR}/Form/*.ui)
file(GLOB INCFILE ${CMAKE_CURRENT_SOURCE_DIR}/Inc/*.h)

set(RESOURCE_FILES
        res.qrc  # 你的资源文件名
)

add_executable(Client main.cpp
        ${UIFILE}
        ${SRCFILE}
        ${INCFILE}
        ${RESOURCE_FILES}
)
target_link_libraries(Client
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
        dwmapi
)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()

    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()

    # platforms 插件（你已有）
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()

    # 核心 DLL（你已有）
    foreach (QT_LIB Core)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach ()

    # ↓↓↓ 新增：整拷 imageformats（含 qjpeg） ↓↓↓
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/imageformats/")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${QT_INSTALL_PATH}/plugins/imageformats"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/imageformats/")
endif ()
