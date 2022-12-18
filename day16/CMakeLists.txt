cmake_minimum_required (VERSION 3.11)

set(SOURCES
    day16.cpp
)

add_executable(day16 ${SOURCES})
target_link_libraries(day16 PRIVATE common nlohmann_json::nlohmann_json)
