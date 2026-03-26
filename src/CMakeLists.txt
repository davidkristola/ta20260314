cmake_minimum_required(VERSION 4.0)
project(technical_assessment)

set(CMAKE_CXX_STANDARD 17)

set(source_files
    event_queue.cpp
    aircraft.cpp
    statistics.cpp
    fault_model.cpp
    sequencer.cpp
    vertiport.cpp
    configuration.cpp
    simulation.cpp
)

add_executable(technical_assessment main.cpp "${source_files}")
target_compile_options(technical_assessment PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wnon-virtual-dtor -Wcast-align -Wunused -Wconversion)
target_include_directories(technical_assessment PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
