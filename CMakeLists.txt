cmake_minimum_required(VERSION 2.6)

project(g5 C CXX)

set(CXX_FLAGS
 -g
 -Wall
 -std=c++11
)

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(src/base)
add_subdirectory(src/net)
add_subdirectory(examples)
