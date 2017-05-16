cmake_minimum_required(VERSION 2.8)

project(sem16)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Werror -Wall")
file(GLOB SOURCES "./*.cpp" "./*.h")
add_executable(${PROJECT_NAME} ${SOURCES})
