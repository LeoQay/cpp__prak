cmake_minimum_required(VERSION 3.20)
project(cpp__prak)

set(CMAKE_CXX_STANDARD 20)

add_executable(cpp_exec 8/83.cpp)

add_executable(test A/test_A3.cpp)

target_link_libraries(test gtest_main)
target_link_libraries(test gtest)
target_link_libraries(test pthread)