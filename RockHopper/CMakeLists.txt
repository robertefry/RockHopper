cmake_minimum_required(VERSION 3.12)
project(RockHopper VERSION 1.0.0)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/RockHopper/*)
add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
