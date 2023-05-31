
# This file contains a list of tools that can be activated and downloaded
# on-demand. Each tool is enabled during the configuration by passing an
# additional `-DUSE_<TOOL>=<VALUE>` argument to CMake.
#
# Optionas are;
#  * USE_OUTPUT       Place compiled binaries in a top-level output directory
#  * USE_CLANG_TIDY   Run clang-tidy on source files after each build
#  * USE_SANITIZERS   Specify a list of sanitizers to compile against
#

include(${CMAKE_CURRENT_LIST_DIR}/get_cpm.cmake)

# ---- output directory ----
set(USE_OUTPUT OFF CACHE BOOL "Place compiled binaries in a top-level output directory")

if(USE_OUTPUT)
  if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/output/bin")
  endif()
  if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    if(UNIX)
      set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/output/lib")
    else()
      set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/output/bin")
    endif()
  endif()
endif()

# ---- compiler cache ----
# This enables CCACHE support through the use of the USE_CCACHE CMake variable
# as a boolean.

set(USE_CCACHE ON CACHE BOOL "Use ccache compilation cache")

if(USE_CCACHE)
  # See https://github.com/TheLartians/Ccache.cmake for more info.
  CPMAddPackage(
    GITHUB_REPOSITORY "TheLartians/Ccache.cmake" GIT_TAG "v1.2.4")

endif()

# ---- clang-tidy ----
# This enables clang-tidy static analysis through the use of the USE_CLANG_TIDY
# CMake variabe as a boolean.

set(USE_CLANG_TIDY ON CACHE BOOL "Use the clang-tidy static analyzer")

if(USE_CLANG_TIDY)
  find_program(CLANG_TIDY NAMES "clang-tidy")
  if(CLANG_TIDY)
    message(STATUS "Found clang-tidy: ${CLANG_TIDY}")
  else()
    message(SEND_ERROR "Cannot enable clang-tidy, executable not found!")
  endif()
endif()

if(USE_CLANG_TIDY AND CLANG_TIDY)
  foreach(LANG "C" "CXX" "OBJC" "OBJCXX")
    set(CMAKE_${LANG}_CLANG_TIDY ${CLANG_TIDY})
  endforeach()
endif()

# ---- sanitizers ----
# This is a list of llvm sanitizers used by declaring the USE_SANITIZERS CMake
# variable as string containing any of:
#  * Address
#  * Memory
#  * MemoryWithOrigins
#  * Undefined
#  * Thread
#  * Leak
#  * CFI
# Multiple values are allowed, e.g. `-DUSE_SANITIZERS=Address,Leak`, but some
# sanitizers cannot be combined together.

set(USE_SANITIZERS "" CACHE STRING "List which llvm sanitizers to use")

if(USE_SANITIZERS)
  # See https://github.com/StableCoder/cmake-scripts for more info.
  CPMAddPackage(
    GITHUB_REPOSITORY "StableCoder/cmake-scripts" GIT_TAG "22.01")

  include(${cmake-scripts_SOURCE_DIR}/sanitizers.cmake)

endif()
