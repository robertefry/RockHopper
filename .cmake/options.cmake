
# This file contains a list of compiler tools and options that can be activated
# on-demand. Each tool is enabled during the configuration by using the cache
# variables prefixed by `OPT_`. For example; `-DOPT_<KEY>=<VALUE>`.

include(${CMAKE_CURRENT_LIST_DIR}/get_cpm.cmake)

#
# Recommended compiler warnings
#
set(OPT_USE_RECOMMENDED_COMPILER_WARNINGS ON
  CACHE BOOL "Use the recommended compiler warnings. (on by default)")

function(opt_use_recommended_compiler_warnings)
  if ((CMAKE_CXX_COMPILER_ID STREQUAL "GNU"  ) OR
      (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  )
    add_compile_options(-Werror)
    add_compile_options(-Wall -Wextra -Wconversion -Wshadow -Wpedantic -Weffc++)
    add_compile_options(-Wno-unused)
  endif()
endfunction()

if (OPT_USE_RECOMMENDED_COMPILER_WARNINGS)
  opt_use_recommended_compiler_warnings()
endif()

#
# In-source builds
#
set(OPT_DISALLOW_IN_SOURCE_BUILDS ON
  CACHE BOOL "Disallow in-source builds. (on by default)")

function(opt_disallow_in_source_builds)
  # make sure the user doesn't play dirty with symlinks
  get_filename_component(srcdir "${CMAKE_SOURCE_DIR}" REALPATH)
  get_filename_component(bindir "${CMAKE_BINARY_DIR}" REALPATH)

  if(${srcdir} STREQUAL ${bindir})
    message("################################################################")
    message("WARNING: in-source builds are explicitly dissalowed.")
    message("Please create a separate build directory and run cmake from there.")
    message("################################################################")
    message(FATAL_ERROR "Quitting configuration.")
  endif()
endfunction()

if(OPT_DISALLOW_IN_SOURCE_BUILDS)
  opt_disallow_in_source_builds()
endif()

#
# Output directory
#
set(OPT_USE_OUTPUT_DIR OFF
  CACHE BOOL "Place compiled binaries in a top-level output directory. (off by default)")

set(OPT_OUTPUT_DIR_BIN "${CMAKE_BINARY_DIR}/output/bin"
  CACHE STRING "The directory to place output libraries.")
set(OPT_OUTPUT_DIR_LIB "${CMAKE_BINARY_DIR}/output/lib"
  CACHE STRING "The directory to place output binaries.")

if(OPT_USE_OUTPUT_DIR)
  if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OPT_OUTPUT_DIR_BIN})
  endif()
  if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OPT_OUTPUT_DIR_LIB})
  endif()
  if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OPT_OUTPUT_DIR_LIB})
  endif()
endif()

#
# Default build type
#
set(OPT_DEFAULT_BUILD_TYPE "Release"
  CACHE STRING "The build type to use at configuration, if nothing is explicitly given.")

function(opt_set_default_build_type build_type)

  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE ${build_type}
      CACHE STRING "The build type on single-config generators." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE
      PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
    message(STATUS "Setting single-config build type, as none was specified.")
  else()
    message(STATUS "Found build type set to '${CMAKE_BUILD_TYPE}'.")
  endif()

  if(NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES ${build_type}
      CACHE STRING "The build types on multi-config generators." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_CONFIGURATION_TYPES
      PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
    message(STATUS "Setting multi-config build types, as none was specified.")
  else()
    message(STATUS "Found configuration types set to '${CMAKE_CONFIGURATION_TYPES}'.")
  endif()

endfunction()

opt_set_default_build_type(${OPT_DEFAULT_BUILD_TYPE})

#
# CCache
#
set(OPT_USE_CCACHE ON
  CACHE BOOL "Use ccache compilation cache. (on by default)")

if(OPT_USE_CCACHE)
  # See https://github.com/TheLartians/Ccache.cmake for more info.
  CPMAddPackage("gh:TheLartians/Ccache.cmake@1.2.4")
endif()

#
# Clang-Tidy
#
set(OPT_USE_CLANG_TIDY ON
  CACHE BOOL "Use the clang-tidy static analyzer. (on by default)")

if(OPT_USE_CLANG_TIDY AND NOT FOUND_CLANG_TIDY)
  find_program(CLANG_TIDY NAMES "clang-tidy")
  if(CLANG_TIDY)
    set(FOUND_CLANG_TIDY TRUE)
    message(STATUS "Found clang-tidy: ${CLANG_TIDY}")
  else()
    message(WARNING "Cannot enable clang-tidy, executable not found!")
  endif()
endif()

# We need to enable clang-tidy for each target individually, to avoid running
# static analysis on code we can't control (e.g. cpm packages)
function(opt_target_enable_clang_tidy target)
  if(OPT_USE_CLANG_TIDY AND CLANG_TIDY)
    foreach(LANG "C" "CXX" "OBJC" "OBJCXX")
      set_target_properties(${target} PROPERTIES ${LANG}_CLANG_TIDY ${CLANG_TIDY})
    endforeach()
  endif()
endfunction()

#
# Sanitizers
#
# This is a list of llvm sanitizers used by declaring the OPT_USE_SANITIZERS
# CMake variable as string (comma-separated list) containing any of:
#  * Address
#  * Memory
#  * MemoryWithOrigins
#  * Undefined
#  * Thread
#  * Leak
#  * CFI
# Multiple values are allowed, e.g. `-DOPT_SANITIZERS=Address,Leak`, but some
# sanitizers cannot be combined together.

set(OPT_SANITIZERS ""
  CACHE STRING "The list of LLVM sanitizers to use.")

if(OPT_SANITIZERS)
  # See https://github.com/StableCoder/cmake-scripts for more info.
  CPMAddPackage("gh:StableCoder/cmake-scripts@23.04")

  include(${cmake-scripts_SOURCE_DIR}/sanitizers.cmake)

endif()

#
# Doxygen
#
set(OPT_USE_DOXYGEN ON
  CACHE BOOL "Generate documentation on Release builds. (on by default)")

find_package(Doxygen)

if(OPT_USE_DOXYGEN)
  if(DOXYGEN_FOUND)
    message(STATUS "Found Doxygen: ${DOXYGEN_EXECUTABLE} version ${DOXYGEN_VERSION}")
  else()
    message(WARNING "Cannot enable Doxygen, executable not found!")
  endif()
endif()

function(opt_target_enable_doxygen target)

  set(DOXYGEN_PROJECT_NAME "${PROJECT_NAME} (${target})")
  set(DOXYGEN_PROJECT_VERSION "${PROJECT_VERSION}")
  set(DOXYGEN_PROJECT_DESCRIPTION "${PROJECT_DESCRIPTION}")
  set(DOXYGEN_INPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
  set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
  file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

  set(DOXYFILE_IN ${CMAKE_SOURCE_DIR}/.doxyfile-in)
  set(DOXYFILE_OUT ${DOXYGEN_OUTPUT_DIR}/.doxyfile)
  configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

  add_custom_target(${target}-docs ALL
    COMMAND ${DOXYGEN_EXECUTABLE} -q ${DOXYFILE_OUT}
    WORKING_DIRECTORY ${DOXYGEN_OUTPUT_DIR}
    COMMENT "Generating documentation for ${target}..."
    VERBATIM)

  if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_dependencies(${target} ${target}-docs)
  endif()

endfunction()
