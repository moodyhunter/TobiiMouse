cmake_minimum_required(VERSION 3.19)
project(TobiiMouse)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

qt_add_executable(TobiiMouse
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/MainWindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/MainWindow.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/MainWindow.ui
    ${CMAKE_CURRENT_LIST_DIR}/src/GazeWorkerThread.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/GazeWorkerThread.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/TobiiDevice.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/TobiiDevice.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/MouseIntegration.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/MouseIntegration.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/NoiseCancellation.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/NoiseCancellation.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/TobiiAPI.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/TobiiAPI.hpp
    )

target_link_libraries(TobiiMouse
    PRIVATE
        Qt::Gui
        Qt::Widgets
        X11
        Xrandr
        Xtst
        ${CMAKE_CURRENT_LIST_DIR}/src/lib/x64/libtobii_stream_engine.so)

