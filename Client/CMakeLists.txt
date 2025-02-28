cmake_minimum_required(VERSION 3.30)
project(CLIENT)

add_executable(CLIENT
        src/client.cpp
)

target_include_directories(CLIENT
        PUBLIC
        include
)