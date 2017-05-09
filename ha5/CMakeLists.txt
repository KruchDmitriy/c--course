cmake_minimum_required(VERSION 2.8)
project(ha2_2_tests)

if(MSVC)
    add_definitions(/W4 /WX)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Werror")
endif()
add_definitions(-DTASK2)

file(GLOB SRC_LIST "./src/*.cpp" "./src/*.h*")

add_executable(${PROJECT_NAME} ${SRC_LIST})
