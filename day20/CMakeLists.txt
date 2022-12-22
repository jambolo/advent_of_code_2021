cmake_minimum_required (VERSION 3.11)

set(SOURCES
    day20.cpp
)

add_executable(day20 ${SOURCES})
target_link_libraries(day20 PRIVATE common nlohmann_json::nlohmann_json)
