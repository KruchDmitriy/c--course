cmake_minimum_required(VERSION 2.8)

project(sem17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")

add_library(mypthread SHARED src/libmypthread.cpp)

add_executable(test_bench ./src/test_bench.cpp)
add_executable(test_recursive ./src/test_recursive.cpp)
add_executable(test_deadlock ./src/test_deadlock.cpp)

target_link_libraries(test_bench dl mypthread)
target_link_libraries(test_recursive dl mypthread)
target_link_libraries(test_deadlock dl mypthread)
target_link_libraries(mypthread dl pthread)
