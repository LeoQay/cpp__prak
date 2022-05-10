cmake_minimum_required(VERSION 3.20)
project(cpp__prak)

set(CMAKE_CXX_STANDARD 20)

add_executable(regex_exec main.cpp regex_parser.cpp token_stream_iterator.cpp)

add_executable(test_regex test.cpp regex_parser.cpp token_stream_iterator.cpp)

target_link_libraries(test_regex gtest_main)
target_link_libraries(test_regex gtest)
target_link_libraries(test_regex pthread)