cmake_minimum_required (VERSION 3.10)

set(SOURCES
    day10.cpp
)

add_executable(day10 ${SOURCES})
target_link_libraries(day10 PRIVATE common nlohmann_json::nlohmann_json)
