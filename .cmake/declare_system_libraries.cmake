
function(declare_system_libraries targets)
  foreach(target ${targets})
    get_target_property(target_include_dirs ${target} INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(target_system_include_dirs ${target} INTERFACE_SYSTEM_INCLUDE_DIRECTORIES)
    get_target_property(target_aliased_name ${target} ALIASED_TARGET)
    if (target_aliased_name)
      set(target ${target_aliased_name})
    endif()
    set_target_properties(${target} PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES ""
      INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${target_system_include_dirs} ${target_include_dirs}")
  endforeach(target)
endfunction()
