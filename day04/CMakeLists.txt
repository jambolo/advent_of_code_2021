cmake_minimum_required (VERSION 3.10)

set(SOURCES
    day04.cpp
)

add_executable(day04 ${SOURCES})
target_link_libraries(day04 PRIVATE nlohmann_json::nlohmann_json)
