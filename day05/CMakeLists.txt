cmake_minimum_required (VERSION 3.10)

set(SOURCES
    day05.cpp
)

add_executable(day05 ${SOURCES})
target_link_libraries(day05 PRIVATE nlohmann_json::nlohmann_json)
