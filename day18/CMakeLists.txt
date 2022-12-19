cmake_minimum_required (VERSION 3.11)

set(SOURCES
    day18.cpp
)

add_executable(day18 ${SOURCES})
target_link_libraries(day18 PRIVATE common nlohmann_json::nlohmann_json)
