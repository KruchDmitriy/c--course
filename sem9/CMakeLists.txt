cmake_minimum_required(VERSION 3.6)
project(sem9)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(sem9 ${SOURCE_FILES})