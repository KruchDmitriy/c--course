cmake_minimum_required(VERSION 2.8)

set(project_name task)
project(${project_name})

file(GLOB SOURCES "./src/*.cpp" "./src/*.h")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Werror -Wall")
add_executable(${project_name} ${SOURCES})
add_executable(task2_1_graph_test_SE ${SOURCES})

target_link_libraries(task2_1_graph_test_SE)
#target_link_libraries(${project_name})

