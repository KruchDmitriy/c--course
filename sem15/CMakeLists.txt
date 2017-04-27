cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Werror -Wall")
file(GLOB SOURCES "./*.cpp" "./*.h")
project(sem15)
add_executable(${PROJECT_NAME} ${SOURCES})
