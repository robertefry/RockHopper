
cmake_minimum_required(VERSION 3.0)
include("vcpkg/scripts/buildsystems/vcpkg.cmake")

# Define project
project("RockHopper" LANGUAGES CXX VERSION 1.0.0)
set(CMAKE_CXX_STANDARD "20")
add_library(${PROJECT_NAME} SHARED "")

# Add tests subdirectory
add_subdirectory(tests)

# Define compiler configuration
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Weffc++ -Wpedantic)
target_compile_options(${PROJECT_NAME} PRIVATE -Wno-unused)

# Define include directories
target_include_directories(${PROJECT_NAME} PUBLIC "include")
target_include_directories(${PROJECT_NAME} PRIVATE "src")

# Define build sources
file(GLOB_RECURSE PROJECT_SOURCES
    "src/*.c" "src/*.cc" "src/*.cpp" "src/*.cxx"
)
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})

# Define ROCKHOPPER_LOG_LEVEL
if(NOT DEFINED ROCKHOPPER_LOG_LEVEL)
    if (DEFINED ${CMAKE_BUILD_TYPE})
        if (${CMAKE_BUILD_TYPE} STREQUAL "MinSizeRel")
            set(ROCKHOPPER_LOG_LEVEL 1)
        elseif (${CMAKE_BUILD_TYPE} STREQUAL "Release")
            set(ROCKHOPPER_LOG_LEVEL 2)
        elseif (${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
            set(ROCKHOPPER_LOG_LEVEL 3)
        elseif (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
            set(ROCKHOPPER_LOG_LEVEL 4)
        endif()
    else()
        set(ROCKHOPPER_LOG_LEVEL 4)
    endif()
endif()
target_compile_definitions(${PROJECT_NAME} PUBLIC ROCKHOPPER_LOG_LEVEL=${ROCKHOPPER_LOG_LEVEL})

# Use threads
find_package(Threads)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_THREAD_LIBS_INIT})

# Use fmtlib
find_package(fmt CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

# Use spdlog
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog spdlog::spdlog_header_only)

# Use glfw3
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw ${GLFW_LIBRARIES} OpenGL ${OPENGL_glu_LIBRARY})

# Use glad
find_package(glad CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC glad::glad)
