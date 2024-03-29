cmake_minimum_required (VERSION 3.10)
project(AdventOfCode-2021 CXX)

option(BUILD_SHARED_LIBS "Build libraries as DLLs" FALSE)

# Common compiler definitions

set(CMAKE_CXX_STANDARD 17)

add_definitions(
    -DNOMINMAX
    -DWIN32_LEAN_AND_MEAN
    -DVC_EXTRALEAN
    -D_CRT_SECURE_NO_WARNINGS
    -D_SECURE_SCL=0
    -D_SCL_SECURE_NO_WARNINGS
)

# Common external libraries

find_package(nlohmann_json REQUIRED)

# Libraries

add_subdirectory(common)

# Solutions

add_subdirectory(day01)
add_subdirectory(day02)
add_subdirectory(day03)
add_subdirectory(day04)
add_subdirectory(day05)
add_subdirectory(day06)
add_subdirectory(day07)
add_subdirectory(day08)
add_subdirectory(day09)
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
add_subdirectory(day25)
