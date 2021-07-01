
cmake_minimum_required(VERSION 3.0)
project("glad")

file(GLOB SOURCES
    "src/glad.c"
    "include/glad/glad.h"
    "include/KHR/khrplatform.h"
)
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC "include")
target_include_directories(${PROJECT_NAME} PRIVATE "src")
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
