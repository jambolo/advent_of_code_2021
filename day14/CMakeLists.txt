cmake_minimum_required (VERSION 3.11)

set(SOURCES
    day14.cpp
)

add_executable(day14 ${SOURCES})
target_link_libraries(day14 PRIVATE common nlohmann_json::nlohmann_json)
