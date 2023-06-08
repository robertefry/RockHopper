
cmake_minimum_required(VERSION 3.12)
enable_testing()

# ---- binary ----

set(TESTS_NAME ${LIBRARY_NAME}-Tests)
add_executable(${TESTS_NAME} "")

target_link_libraries(${TESTS_NAME} PRIVATE ${LIBRARY_NAME})

# ---- cmake targets ----

add_custom_target(test-${LIBRARY_NAME}
  COMMAND $<TARGET_FILE:${TESTS_NAME}>
  DEPENDS ${TESTS_NAME})

add_test(NAME ${TESTS_NAME} COMMAND $<TARGET_FILE:${TESTS_NAME}>)

# ---- compiler targets ----

target_include_directories(${TESTS_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

file(GLOB_RECURSE GLOB_SOURCES CONFIGURE_DEPENDS
  "src/*.c" "src/*.cc" "src/*.cpp" "src/*.cxx")
target_sources(${TESTS_NAME} PRIVATE ${GLOB_SOURCES})

target_link_libraries(${TESTS_NAME} PUBLIC Catch2::Catch2WithMain)
