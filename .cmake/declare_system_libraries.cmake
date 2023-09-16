
# The declare_system_library function changes the relavent target properties
# such that the CMake compiler sees it as a system dependency.

function(opt_declare_system_library target)
  message(STATUS "Declaring system library ${target}")
  get_target_property(target_aliased_name ${target} ALIASED_TARGET)
  if (target_aliased_name)
    set(target ${target_aliased_name})
  endif()
  set_target_properties(${target} PROPERTIES
    INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${target},INTERFACE_INCLUDE_DIRECTORIES>)
endfunction()

function(opt_declare_system_libraries targets)
  foreach(target ${targets})
    opt_declare_system_library(${target})
  endforeach()
endfunction()
