cmake_minimum_required (VERSION 3.11)

set(SOURCES
    day22-1.cpp
    day22-2.cpp
    Cube.cpp
    Cube.h
)

add_executable(day22-1 day22-1.cpp Cube.cpp Cube.h)
target_link_libraries(day22-1 PRIVATE common nlohmann_json::nlohmann_json)

add_executable(day22-2 day22-2.cpp Cube.cpp Cube.h)
target_link_libraries(day22-2 PRIVATE common nlohmann_json::nlohmann_json)
