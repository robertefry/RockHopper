cmake_minimum_required(VERSION 3.7.2)

# ---- project ----
include(.cmake/options.cmake)
include(.cmake/declare_project_targets.cmake)

project("RockHopper" VERSION "1.0.0" LANGUAGES "C" "CXX"
  HOMEPAGE_URL "https://github.com/robertefry/RockHopper"
  DESCRIPTION "A C++ multithreaded logic and rendering framework.")

set(CMAKE_CXX_STANDARD "20")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example
# when compiling with PCH enabled
set(CMAKE_CXX_EXTENSIONS OFF)
#
# ---- add dependencies via CPM ----
# see https://github.com/cpm-cmake/CPM.cmake for more info
include(.cmake/get_cpm.cmake)
include(.cmake/declare_system_libraries.cmake)

CPMAddPackage(NAME "Catch2"
  GITHUB_REPOSITORY "catchorg/Catch2" GIT_TAG "v3.3.2")
opt_declare_system_libraries(Catch2::Catch2 Catch2::Catch2WithMain)

CPMAddPackage(NAME "fmt"
  GITHUB_REPOSITORY "fmtlib/fmt" GIT_TAG "9.1.0"
  OPTIONS "FMT_INSTALL NO")
CPMAddPackage(NAME "spdlog"
  GITHUB_REPOSITORY "gabime/spdlog" GIT_TAG "v1.11.0"
  OPTIONS "SPDLOG_FMT_EXTERNAL ON")
opt_declare_system_libraries(fmt::fmt spdlog::spdlog)

CPMAddPackage(NAME concurrentqueue
  GITHUB_REPOSITORY "cameron314/concurrentqueue" GIT_TAG "v1.0.3")
opt_declare_system_libraries(concurrentqueue)

CPMAddPackage(NAME GLFW
  GITHUB_REPOSITORY "glfw/glfw" GIT_TAG "3.3.2"
  OPTIONS
    "GLFW_BUILD_TESTS OFF"
    "GLFW_BUILD_EXAMPLES OFF"
    "GLFW_BULID_DOCS OFF")
opt_declare_system_libraries(glfw)

# ---- project targets ----

add_subdirectory(RockHopper)
add_subdirectory(RockHopper-Sandbox)

# ---- project packaged targets ----
# this allows users to install and find the library via `find_package`.
CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.11.0")
