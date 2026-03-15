cmake_minimum_required(VERSION 4.0)
project(technical_assessment)

set(CMAKE_CXX_STANDARD 17)

set(source_files
)

add_executable(technical_assessment main.cpp "${source_files}")

target_include_directories(technical_assessment PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
