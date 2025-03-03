cmake_minimum_required(VERSION 3.30)
project(TCP)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(Server)
add_subdirectory(Client)

