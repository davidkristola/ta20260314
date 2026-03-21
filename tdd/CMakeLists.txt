cmake_minimum_required(VERSION 4.0)
project(ta_tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

find_package(GTest REQUIRED)

set(source_files
    ../src/event_queue.cpp
    ../src/aircraft.cpp
    ../src/statistics.cpp
    ../src/fault_model.cpp
    ../src/sequencer.cpp
    tests/test_event_queue.cpp
    tests/test_event_type.cpp
    tests/test_aircraft_type.cpp
    tests/test_aircraft.cpp
    tests/test_statistics.cpp
    tests/test_fault_model.cpp
    tests/test_sequencer.cpp
)

add_executable(ta_tests main.cpp "${source_files}")
target_link_libraries(ta_tests GTest::gtest)
include_directories("../src")
