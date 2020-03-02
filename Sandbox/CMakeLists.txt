cmake_minimum_required(VERSION 3.12)
project(Sandbox VERSION 1.0.0)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/Sandbox/*)
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE RockHopper)
