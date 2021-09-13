cmake_minimum_required(VERSION 3.10)

project(RayTracer)

add_subdirectory(src)
add_subdirectory(extern)
add_subdirectory(app)

option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
  enable_testing()
  include(GoogleTest)
  add_subdirectory(tests)
endif()