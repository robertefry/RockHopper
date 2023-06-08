
#
# Define the recommended set of build flags
#
function(define_recommended_build_flags)
  if ((CMAKE_CXX_COMPILER_ID STREQUAL "GNU"  ) OR
      (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  )
    add_compile_options(-Werror)
    add_compile_options(-Wall -Wextra -Wconversion -Wshadow -Wpedantic -Weffc++)
    add_compile_options(-Wno-unused)
  endif()
endfunction()
define_recommended_build_flags()

#
# Dissallow in-source builds
#
function(prevent_in_source_builds)
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
prevent_in_source_builds()

#
# Set the default build type
#
function(default_build_type build_type)

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
default_build_type("Release")
