cmake_minimum_required (VERSION 3.11)

set(SOURCES
    day12.cpp
)

add_executable(day12 ${SOURCES})
target_link_libraries(day12 PRIVATE common nlohmann_json::nlohmann_json)
