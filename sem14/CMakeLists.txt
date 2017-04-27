cmake_minimum_required(VERSION 3.7)
project(sem14)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES src/main.cpp src/faults.cpp
        src/bin_search_tree.h)
add_executable(sem14 ${SOURCE_FILES})