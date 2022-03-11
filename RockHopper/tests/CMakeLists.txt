
cmake_minimum_required(VERSION 3.11)

# Define project
project("RockHopper_Tests" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD "20")
add_executable(${PROJECT_NAME} "")

# Define compiler configuration
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Weffc++ -Wpedantic)
target_compile_options(${PROJECT_NAME} PRIVATE -Wno-unused)

# Define include directories
target_include_directories(${PROJECT_NAME} PUBLIC "include")
target_include_directories(${PROJECT_NAME} PRIVATE "src")

# Define test sources
file(GLOB_RECURSE TEST_SOURCES
    "src/*.c" "src/*.cc" "src/*.cpp" "src/*.cxx"
)
target_sources(${PROJECT_NAME} PRIVATE ${TEST_SOURCES})

# Use RockHopper
target_link_libraries(${PROJECT_NAME} PRIVATE RockHopper)

# Use GTest
find_package(GTest CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

# Add test target
add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}>)

# Add explicit gtest target
add_custom_target(gtest
    COMMAND $<TARGET_FILE:${PROJECT_NAME}>
    DEPENDS ${PROJECT_NAME})
