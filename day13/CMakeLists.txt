cmake_minimum_required (VERSION 3.11)

set(SOURCES
    day13.cpp
)

add_executable(day13 ${SOURCES})
target_link_libraries(day13 PRIVATE common nlohmann_json::nlohmann_json)
