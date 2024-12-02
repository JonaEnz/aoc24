cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

project(
  AdventOfCode24
  VERSION 0.0.1
  DESCRIPTION ""
  LANGUAGES CXX C
)

include_directories(${PROJECT_SOURCE_DIR}/utils)

add_subdirectory(utils)
add_subdirectory(day1)
add_subdirectory(day2)
