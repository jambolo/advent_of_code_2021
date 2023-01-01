cmake_minimum_required (VERSION 3.11)

set(SOURCES
    day25.cpp
)

add_executable(day25 ${SOURCES})
target_link_libraries(day25 PRIVATE common nlohmann_json::nlohmann_json)
