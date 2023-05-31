cmake_minimum_required(VERSION 3.7.2)

# ---- project ----

project("RockHopper" VERSION "1.0.0" LANGUAGES "C" "CXX"
  HOMEPAGE_URL "https://github.com/robertefry/RockHopper"
  DESCRIPTION "A C++ multithreaded logic and rendering framework.")

set(CMAKE_CXX_STANDARD "20")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ---- add dependencies via CPM ----
# see https://github.com/cpm-cmake/CPM.cmake for more info
include(.cmake/get_cpm.cmake)
include(.cmake/declare_system_libraries.cmake)

CPMAddPackage(NAME "Catch2"
  GITHUB_REPOSITORY "catchorg/Catch2" GIT_TAG "v3.3.2"
)

CPMAddPackage(NAME "fmt"
  GITHUB_REPOSITORY "fmtlib/fmt" GIT_TAG "9.1.0"
  OPTIONS "FMT_INSTALL NO"
)
CPMAddPackage(NAME "spdlog"
  GITHUB_REPOSITORY "gabime/spdlog" GIT_TAG "v1.11.0"
  OPTIONS "SPDLOG_FMT_EXTERNAL ON"
)
declare_system_libraries(fmt::fmt spdlog::spdlog)

CPMAddPackage(NAME Boost
  GITHUB_REPOSITORY "boostorg/boost" GIT_TAG "boost-1.81.0"
)
CPMAddPackage(NAME concurrentqueue
  GITHUB_REPOSITORY "cameron314/concurrentqueue" GIT_TAG "v1.0.3"
)

# ---- project units ----
include(.cmake/build_tools.cmake) # toolkit for buildsystem features

add_subdirectory(RockHopper)
add_subdirectory(RockHopper-Sandbox)

# ---- project installable targets ----
# this allows users to install and find the library via `find_package`.
CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.10.0")

# ---- cmake & compiler options ----
include(.cmake/build_type.cmake)

# dissallow in-source builds
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR
    "In-source build are disallowed, please run CMake from a build directory.")
endif()

# set cxx compiler flags
if ((CMAKE_CXX_COMPILER_ID STREQUAL "GNU"  ) OR
    (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
)
  add_compile_options(-Werror)
  add_compile_options(-Wall -Wextra -Wconversion -Wshadow -Wpedantic -Weffc++)
  add_compile_options(-Wno-unused)
endif()
