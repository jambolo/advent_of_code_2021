cmake_minimum_required (VERSION 3.10)

set(SOURCES
    day07.cpp
)

add_executable(day07 ${SOURCES})
target_link_libraries(day07 PRIVATE common nlohmann_json::nlohmann_json)
