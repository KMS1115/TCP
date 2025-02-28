cmake_minimum_required(VERSION 3.30)
project(SERVER)

add_executable(SERVER
        src/server.cpp
)

target_include_directories(SERVER
        PUBLIC
        include
)

target_link_libraries(SERVER
        Common
)