
# This file contains a list of tools and build options that can be activated
# and downloaded on-demand. Each tool is enabled during the configuration by
# passing an additional `-DUSE_<KEY>=<VALUE>` argument to CMake.
#
# Optional `KEY`s are;
#  * USE_OUTPUT_DIR   Place compiled binaries in a top-level output directory
#  * USE_CCACHE       Use the ccache compilation cache
#  * USE_CLANG_TIDY   Run clang-tidy on source files after each build
#  * USE_SANITIZERS   Specify a list of sanitizers to compile against
#

include(${CMAKE_CURRENT_LIST_DIR}/get_cpm.cmake)

# ---- output directory ----
set(USE_OUTPUT_DIR OFF
  CACHE BOOL "Place compiled binaries in a top-level output directory.")

set(USE_OUTPUT_DIR_LIB "${CMAKE_BINARY_DIR}/output/lib"
  CACHE STRING "The directory to place output binaries.")
set(USE_OUTPUT_DIR_BIN "${CMAKE_BINARY_DIR}/output/bin"
  CACHE STRING "The directory to place output libraries.")

if(USE_OUTPUT_DIR)
  if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${USE_OUTPUT_DIR_BIN})
  endif()
  if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${USE_OUTPUT_DIR_LIB})
  endif()
endif()

# ---- compiler cache ----
# This enables CCACHE support through the use of the USE_CCACHE CMake variable
# as a boolean.
set(USE_CCACHE ON
  CACHE BOOL "Use ccache compilation cache.")

if(USE_CCACHE)
  # See https://github.com/TheLartians/Ccache.cmake for more info.
  CPMAddPackage("gh:TheLartians/Ccache.cmake@1.2.4")
endif()

# ---- clang-tidy ----
# This enables clang-tidy static analysis through the use of the USE_CLANG_TIDY
# CMake variabe as a boolean.
set(USE_CLANG_TIDY ON
  CACHE BOOL "Use the clang-tidy static analyzer.")

if(USE_CLANG_TIDY AND NOT FOUND_CLANG_TIDY)
  find_program(CLANG_TIDY NAMES "clang-tidy")
  if(CLANG_TIDY)
    set(FOUND_CLANG_TIDY TRUE)
    message(STATUS "Found clang-tidy: ${CLANG_TIDY}")
  else()
    message(SEND_ERROR "Cannot enable clang-tidy, executable not found!")
  endif()
endif()

function(try_target_enable_clang_tidy target)
  if(USE_CLANG_TIDY AND CLANG_TIDY)
    foreach(LANG "C" "CXX" "OBJC" "OBJCXX")
      set_target_properties(${target} PROPERTIES ${LANG}_CLANG_TIDY ${CLANG_TIDY})
    endforeach()
  endif()
endfunction()

# ---- sanitizers ----
# This is a list of llvm sanitizers used by declaring the USE_SANITIZERS CMake
# variable as string (comma-separated list) containing any of:
#  * Address
#  * Memory
#  * MemoryWithOrigins
#  * Undefined
#  * Thread
#  * Leak
#  * CFI
# Multiple values are allowed, e.g. `-DUSE_SANITIZERS=Address,Leak`, but some
# sanitizers cannot be combined together.
set(USE_SANITIZERS ""
  CACHE STRING "List which llvm sanitizers to use.")

if(USE_SANITIZERS)
  # See https://github.com/StableCoder/cmake-scripts for more info.
  CPMAddPackage("gh:StableCoder/cmake-scripts@23.04")

  include(${cmake-scripts_SOURCE_DIR}/sanitizers.cmake)

endif()
