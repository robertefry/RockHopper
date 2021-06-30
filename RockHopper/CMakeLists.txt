
cmake_minimum_required(VERSION 3.0)

# Define project
project("RockHopper" LANGUAGES CXX VERSION 1.0.0)
set(CMAKE_CXX_STANDARD "20")
enable_testing()

# Define build target
file(GLOB_RECURSE SOURCES
    "src/*.c" "src/*.cc" "src/*.cpp" "src/*.cxx"
)
add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC "include")
target_include_directories(${PROJECT_NAME} PRIVATE "src")

# Define compiler configuration
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Weffc++ -Wpedantic)
target_compile_options(${PROJECT_NAME} PRIVATE -Wno-unused-parameter -Wno-unused-variable)

# Use threads
find_package(Threads)
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

# Use fmtlib
find_package(fmt)
target_link_libraries(${PROJECT_NAME} fmt::fmt)

# Use spdlog
add_subdirectory("vendor/spdlog")
target_link_libraries(${PROJECT_NAME} spdlog)
target_include_directories(${PROJECT_NAME} PRIVATE "vendor/spdlog/include")
set_target_properties(spdlog PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Use glfw
add_subdirectory("vendor/glfw")
target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES} OpenGL ${OPENGL_glu_LIBRARY})
target_include_directories(${PROJECT_NAME} PUBLIC "vendor/glfw/include")
set_target_properties(glfw PROPERTIES POSITION_INDEPENDENT_CODE ON)
