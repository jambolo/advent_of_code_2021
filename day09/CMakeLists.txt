cmake_minimum_required (VERSION 3.10)

set(SOURCES
    day09.cpp
)

add_executable(day09 ${SOURCES})
target_link_libraries(day09 PRIVATE common nlohmann_json::nlohmann_json)
