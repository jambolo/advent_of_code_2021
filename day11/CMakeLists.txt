cmake_minimum_required (VERSION 3.11)

set(SOURCES
    day11.cpp
)

add_executable(day11 ${SOURCES})
target_link_libraries(day11 PRIVATE common nlohmann_json::nlohmann_json)
