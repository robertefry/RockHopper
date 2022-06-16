
cmake_minimum_required(VERSION 3.0)

# Define binary
set(BINARY_NAME "RockHopper-Sandbox")
add_executable(${BINARY_NAME} "")

# Define compiler configuration
target_compile_options(${BINARY_NAME} PRIVATE -Wall -Wextra -Weffc++ -Wpedantic)
target_compile_options(${BINARY_NAME} PRIVATE -Wno-unused)

# Define include directories
target_include_directories(${BINARY_NAME} PUBLIC "include")
target_include_directories(${BINARY_NAME} PRIVATE "src")

# Define build sources
file(GLOB_RECURSE BINARY_SOURCES
  "src/*.c" "src/*.cc" "src/*.cpp" "src/*.cxx"
)
target_sources(${BINARY_NAME} PRIVATE ${BINARY_SOURCES})

# Use RockHopper
target_link_libraries(${BINARY_NAME} PRIVATE RockHopper)

# Add launch target
add_custom_target(run-Sandbox
  COMMENT "Running the ${BINARY_NAME} binary..."
  DEPENDS ${BINARY_NAME}
  COMMAND $<TARGET_FILE:${BINARY_NAME}>)
