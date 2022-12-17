cmake_minimum_required (VERSION 3.11)

set(SOURCES
    day15.cpp
)

add_executable(day15 ${SOURCES})
target_link_libraries(day15 PRIVATE common nlohmann_json::nlohmann_json)
