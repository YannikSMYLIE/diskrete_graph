cmake_minimum_required(VERSION 3.8)
project(diskrete_graph)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp graph.cpp)
add_executable(diskrete_graph ${SOURCE_FILES})