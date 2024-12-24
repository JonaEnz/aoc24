cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 23)

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
add_subdirectory(day3)
add_subdirectory(day4)
add_subdirectory(day5)
add_subdirectory(day6)
add_subdirectory(day7)
add_subdirectory(day8)
add_subdirectory(day9)
add_subdirectory(day10)
add_subdirectory(day11)
add_subdirectory(day12)
add_subdirectory(day13)
add_subdirectory(day14)
add_subdirectory(day15)
add_subdirectory(day16)
add_subdirectory(day17)
add_subdirectory(day18)
add_subdirectory(day19)
add_subdirectory(day20)
add_subdirectory(day21)
add_subdirectory(day22)
add_subdirectory(day23)
add_subdirectory(day24)
