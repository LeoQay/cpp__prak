cmake_minimum_required(VERSION 3.20)
project(93)

set(CMAKE_CXX_STANDARD 20)

add_executable(93 main.cpp)

target_link_libraries(93 gtest_main)
target_link_libraries(93 gtest)
target_link_libraries(93 pthread)
