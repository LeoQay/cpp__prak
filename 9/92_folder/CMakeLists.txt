cmake_minimum_required(VERSION 3.20)
project(cpp_prak)

set(CMAKE_CXX_STANDARD 20)

add_executable(test_cpp_prak chernovik.cpp)

target_link_libraries(test_cpp_prak gtest_main)
target_link_libraries(test_cpp_prak gtest)
target_link_libraries(test_cpp_prak pthread)
