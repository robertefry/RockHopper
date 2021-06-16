
cmake_minimum_required(VERSION 3.0)

project(RockHopper_META)

# Define the output directory
set(PROJECT_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIRECTORY})

# Add RockHopper
add_subdirectory(RockHopper)
include_directories(RockHopper/include)

# Add Sandbox
add_subdirectory(Sandbox)
