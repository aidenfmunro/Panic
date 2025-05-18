cmake_minimum_required(VERSION 3.16)
project(panic LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 COMPONENTS Core Gui Widgets Charts Concurrent REQUIRED)

qt_standard_project_setup()

# target_compile_options(${TARGET} PRIVATE -fsanitize=address -g)
target_link_options(${TARGET} PRIVATE -lpthread)

enable_testing()

target_include_directories(panic PRIVATE include)

target_link_libraries(panic PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Charts
    Qt6::Concurrent
)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "AddressSanitizer enabled")
    target_compile_options(panic PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    target_link_libraries(panic PRIVATE -fsanitize=address)
endif()

target_compile_options(panic PRIVATE -Wall -Wextra -pedantic)
