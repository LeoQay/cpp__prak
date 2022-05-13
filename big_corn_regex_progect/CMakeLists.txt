cmake_minimum_required(VERSION 3.20)
project(cpp__prak)

set(CMAKE_CXX_STANDARD 20)

add_executable(regex_exec B2.cpp
        regex_parser.cpp regex_parser.h
        token_stream_iterator.cpp token_stream_iterator.h
        regex_checker.cpp regex_checker.h
        left_grm_to_dfa.cpp left_grm_to_dfa.h
        delete_empty_rules.cpp delete_empty_rules.h
        global_typedef.h global_typedef.cpp
        delete_bad.cpp delete_bad.h)

add_executable(test_regex test.cpp
        regex_parser.cpp regex_parser.h
        token_stream_iterator.cpp token_stream_iterator.h
        regex_checker.cpp regex_checker.h
        left_grm_to_dfa.cpp left_grm_to_dfa.h
        global_typedef.h
        delete_bad.cpp delete_bad.h)

target_link_libraries(test_regex gtest_main)
target_link_libraries(test_regex gtest)
target_link_libraries(test_regex pthread)