
include(.cmake/project_options.cmake) # toolkit of project options

function(declare_project_targets targets)
  foreach(target ${targets})
    declare_project_target(${target})
  endforeach()
endfunction()

function(declare_project_target target)
  try_target_enable_clang_tidy(${target})
endfunction()
