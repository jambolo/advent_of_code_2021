cmake_minimum_required (VERSION 3.10)
project(AdventOfCode-2021 CXX)

option(BUILD_SHARED_LIBS "Build libraries as DLLs" FALSE)

# Common compiler definitions

add_definitions(
    -DNOMINMAX
    -DWIN32_LEAN_AND_MEAN
    -DVC_EXTRALEAN
    -D_CRT_SECURE_NO_WARNINGS
    -D_SECURE_SCL=0
    -D_SCL_SECURE_NO_WARNINGS
)

# Common external libraries

#find_package(nlohmann_json REQUIRED)

# Libraries


# Solutions

add_subdirectory(day01)