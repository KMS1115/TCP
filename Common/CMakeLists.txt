cmake_minimum_required(VERSION 3.30)
project(COMMON)

add_library(COMMON
        src/common.cpp
)

target_include_directories(COMMON
        PUBLIC
        include
)