cmake_minimum_required(VERSION 3.12)

# ---- project ----
project("RockHopper" VERSION "1.0.0" LANGUAGES "C" "CXX"
  HOMEPAGE_URL "https://github.com/robertefry/RockHopper"
  DESCRIPTION "A C++ multithreaded logic and rendering framework.")

set(CMAKE_CXX_STANDARD "20")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ---- standards ----
# see https://github.com/robertefry/RockHopperStandards for more info
include(.cmake/get_cpm.cmake)
CPMAddPackage("gh:robertefry/RockHopperStandards@1.3.6")

# ---- project targets ----

add_subdirectory(RockHopper)
add_subdirectory(RockHopper-Sandbox)

# ---- project packaged targets ----
# this allows users to install and find the library via `find_package`.
CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.11.0")
