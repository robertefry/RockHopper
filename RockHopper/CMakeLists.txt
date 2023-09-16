cmake_minimum_required(VERSION 3.12)

# ---- binary ----

set(LIBRARY_NAME "RockHopper")
add_library(${LIBRARY_NAME} SHARED "")
opt_declare_project_target(${LIBRARY_NAME})

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

target_link_libraries(${LIBRARY_NAME} PUBLIC fmt::fmt spdlog::spdlog)
target_link_libraries(${LIBRARY_NAME} PUBLIC concurrentqueue)

target_link_libraries(${LIBRARY_NAME} PRIVATE glfw)

# ---- testing ----

add_subdirectory(tests)
