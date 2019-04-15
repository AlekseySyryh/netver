# cmake_minimum_required(VERSION <specify CMake version here>)
project(netver)

set(CMAKE_CXX_STANDARD 14)

add_executable(netver main.cpp)

target_link_libraries(netver -static)