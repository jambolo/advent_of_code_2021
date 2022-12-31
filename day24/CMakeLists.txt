cmake_minimum_required (VERSION 3.11)

set(SOURCES
    day24.cpp
)

add_executable(day24 ${SOURCES})
target_link_libraries(day24 PRIVATE common nlohmann_json::nlohmann_json)
