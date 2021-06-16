
cmake_minimum_required(VERSION 3.0)

# Define project
project("RockHopper_Sandbox" LANGUAGES CXX VERSION 1.0.0)
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
add_executable(${PROJECT_NAME} ${SOURCES})

# Use RockHopper
# add_subdirectory(RockHopper)
target_link_libraries(${PROJECT_NAME} PRIVATE RockHopper)

# Add launch target
add_custom_target(run_sandbox
    COMMENT "Running the ${PROJECT_NAME} binary..."
    COMMAND $<TARGET_FILE:${PROJECT_NAME}>
    DEPENDS ${PROJECT_NAME})
add_dependencies(run_sandbox RockHopper)
