
# The declare_project_target function sets up any additional options for the
# given target such that it adheres to the project specification.

include(.cmake/options.cmake)

function(opt_declare_project_target target)
  opt_target_enable_clang_tidy(${target})
  opt_target_enable_doxygen(${target})
endfunction()

function(opt_declare_project_targets targets)
  foreach(target ${targets})
    opt_declare_project_target(${target})
  endforeach()
endfunction()
