cmake_minimum_required (VERSION 3.11)

set(SOURCES
    day23.cpp
)

add_executable(day23 day23.cpp)
target_link_libraries(day23 PRIVATE common nlohmann_json::nlohmann_json)
