cmake_minimum_required(VERSION 3.12)

# ---- binary ----

set(LIBRARY_NAME "RockHopper")
add_library(${LIBRARY_NAME} SHARED "")

target_compile_features(${LIBRARY_NAME} PRIVATE cxx_std_20)
set_target_properties(${LIBRARY_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_subdirectory(tests)

# ---- compiler targets ----

target_include_directories(${LIBRARY_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

file(GLOB_RECURSE GLOB_SOURCES CONFIGURE_DEPENDS
  "src/*.c" "src/*.cc" "src/*.cpp" "src/*.cxx")
target_sources(${LIBRARY_NAME} PRIVATE ${GLOB_SOURCES})

# ---- dependencies ----

CPMAddPackage(NAME "fmt"
  GITHUB_REPOSITORY "fmtlib/fmt" GIT_TAG "9.1.0"
  OPTIONS "FMT_INSTALL NO"
)
target_link_libraries(${LIBRARY_NAME} PUBLIC fmt::fmt)

CPMAddPackage(NAME "spdlog"
  GITHUB_REPOSITORY "gabime/spdlog" GIT_TAG "v1.11.0"
  OPTIONS "SPDLOG_FMT_EXTERNAL ON"
)
target_link_libraries(${LIBRARY_NAME} PUBLIC spdlog::spdlog)

CPMAddPackage(NAME concurrentqueue
  GITHUB_REPOSITORY "cameron314/concurrentqueue" GIT_TAG "v1.0.3"
)
target_link_libraries(${LIBRARY_NAME} PUBLIC concurrentqueue)

# CPMAddPackage(NAME glad
#   GITHUB_REPOSITORY "Dav1dde/glad" GIT_TAG "v2.0.8"
# )
# list(APPEND CMAKE_MODULE_PATH "${glad_SOURCE_DIR}/cmake")
# include(GladConfig)
# glad_add_library(glad_gl_core_33 REPRODUCIBLE API gl:core=3.3)
# target_link_libraries(${LIBRARY_NAME} PRIVATE glad_gl_core_33)

CPMAddPackage(NAME GLFW
  GITHUB_REPOSITORY "glfw/glfw" GIT_TAG "3.3.2"
  OPTIONS
    "GLFW_BUILD_TESTS OFF"
    "GLFW_BUILD_EXAMPLES OFF"
    "GLFW_BULID_DOCS OFF"
)
target_link_libraries(${LIBRARY_NAME} PRIVATE glfw)
