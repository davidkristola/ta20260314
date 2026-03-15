cmake_minimum_required(VERSION 4.0)
project(ta_tests)

set(CMAKE_CXX_STANDARD 17)

find_package(GTest REQUIRED)

set(source_files
    tests/test_foo.cpp
)

add_executable(ta_tests main.cpp "${source_files}")
target_link_libraries(ta_tests GTest::gtest)
#include_directories("../src")
