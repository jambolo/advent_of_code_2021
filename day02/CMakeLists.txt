cmake_minimum_required (VERSION 3.10)

set(SOURCES
    day02.cpp
)

add_executable(day02 ${SOURCES})
target_link_libraries(day02 PRIVATE nlohmann_json::nlohmann_json)
