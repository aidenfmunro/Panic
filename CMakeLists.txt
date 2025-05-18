cmake_minimum_required(VERSION 3.16)
project(panic LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 REQUIRED COMPONENTS Widgets Concurrent Charts)

qt_standard_project_setup()

qt_add_executable(panic
    src/main.cpp
    src/MainWindow.cpp include/MainWindow.h
    src/MonitorController.cpp include/MonitorController.h
    src/PingWorker.cpp include/PingWorker.h
    src/ChartWindow.cpp include/ChartWindow.h
)

target_include_directories(panic PRIVATE include)

target_link_libraries(panic PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Concurrent
    Qt6::Charts
)
