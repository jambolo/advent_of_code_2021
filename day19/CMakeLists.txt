cmake_minimum_required (VERSION 3.11)

set(SOURCES
    day19.cpp
)

add_executable(day19 ${SOURCES})
target_link_libraries(day19 PRIVATE common nlohmann_json::nlohmann_json)
