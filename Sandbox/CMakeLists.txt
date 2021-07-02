
cmake_minimum_required(VERSION 3.0)

# Define project
project("RockHopper_Sandbox" LANGUAGES CXX VERSION 1.0.0)
set(CMAKE_CXX_STANDARD "20")
add_executable(${PROJECT_NAME} "")
enable_testing()

# Define compiler configuration
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Weffc++ -Wpedantic)
target_compile_options(${PROJECT_NAME} PRIVATE -Wno-unused)

# Define include directories
target_include_directories(${PROJECT_NAME} PUBLIC "include")
target_include_directories(${PROJECT_NAME} PRIVATE "src")

# Define build sources
file(GLOB_RECURSE SOURCES
    "src/*.c" "src/*.cc" "src/*.cpp" "src/*.cxx"
)
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})

# Use RockHopper
target_link_libraries(${PROJECT_NAME} PRIVATE RockHopper)

# Add launch target
add_custom_target(run_sandbox
    COMMENT "Running the ${PROJECT_NAME} binary..."
    COMMAND $<TARGET_FILE:${PROJECT_NAME}>
    DEPENDS ${PROJECT_NAME})
add_dependencies(run_sandbox RockHopper)
