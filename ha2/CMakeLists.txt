cmake_minimum_required(VERSION 2.8)
project(ha2_tests)

if(MSVC)
    add_definitions(/W4 /WX)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror")
endif()

file(GLOB SRC_LIST "./src/*.cpp" "./src/*.h")

#SET(SRC_LIST ./src/main.cpp ./src/shared_buffer.h ./src/shared_buffer.cpp ./src/big_int.h ./src/big_int.cpp ./src/vector.h
#        ./src/rational.cpp ./src/rational.h)

add_executable(${PROJECT_NAME} ${SRC_LIST})
