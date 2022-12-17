cmake_minimum_required (VERSION 3.10)

set(SOURCES
    expect.h
    expect.cpp
    PathFinder.h
    PathFinder.cpp
)

set(INTERFACE_INCLUDE_PATHS
    ${PROJECT_SOURCE_DIR}
)

add_library(common ${SOURCES})
target_include_directories(common INTERFACE ${INTERFACE_INCLUDE_PATHS})
