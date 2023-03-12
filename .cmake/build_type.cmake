
# Set a default build type if none was specified
set(default_build_type "Release")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
else()
  if(CMAKE_BUILD_TYPE)
    message(STATUS "Found build type set to '${CMAKE_BUILD_TYPE}'.")
  elseif(CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Found configuration types set to '${CMAKE_CONFIGURATION_TYPES}'.")
  endif()
endif()
