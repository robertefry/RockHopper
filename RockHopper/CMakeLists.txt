
cmake_minimum_required(VERSION 3.0)

# Define project
project("RockHopper" LANGUAGES CXX VERSION 1.0.0)
set(CMAKE_CXX_STANDARD "20")
enable_testing()

# Define flags
set(CMAKE_CXX_FLAGS_DEBUG_INIT   "-Wall -Wextra -Weffc++ -pedantic -pthread -std=c++20 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall -Wextra -Weffc++ -pedantic -pthread -std=c++20")

# Glob sources
file(GLOB_RECURSE SOURCES
    "src/*.c" "src/*.cc" "src/*.cpp" "src/*.cxx"
)

# Add build target
add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC "include")

# Use spdlog
target_include_directories(${PROJECT_NAME} PRIVATE "spdlog/include")
