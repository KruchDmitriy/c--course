cmake_minimum_required(VERSION 2.8)
project(sem10)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Werror")

file(GLOB SRC_LIST "./*.cpp" "./*.h*")
add_executable(${PROJECT_NAME} "main.cpp" "imstring.h")
