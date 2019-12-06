cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 11)

project(bigint)

# library for working with big integers
set(SOURCE_LIB bigint.cpp)
add_library(bigint STATIC ${SOURCE_LIB})

# tests for big integer library
add_executable(test test.cpp)
target_link_libraries(test bigint)

# CLI interface for big integers
add_executable(bigint_main main.cpp)
target_link_libraries(bigint_main bigint)