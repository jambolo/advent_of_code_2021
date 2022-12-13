cmake_minimum_required (VERSION 3.10)

set(SOURCES
    day08.cpp
)

add_executable(day08 ${SOURCES})
target_link_libraries(day08 PRIVATE common nlohmann_json::nlohmann_json)
