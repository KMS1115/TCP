cmake_minimum_required(VERSION 3.30)
project(TCP)

find_package(Qt5 REQUIRED COMPONENTS Core Network)

set(CMAKE_CXX_STANDARD 20)

add_executable(TCP main.cpp)

target_link_libraries(TCP
        Qt5::Core
        Qt5::Network
)