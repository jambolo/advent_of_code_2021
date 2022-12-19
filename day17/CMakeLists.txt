cmake_minimum_required (VERSION 3.11)

set(SOURCES
    day17.cpp
)

add_executable(day17 ${SOURCES})
target_link_libraries(day17 PRIVATE common nlohmann_json::nlohmann_json)
