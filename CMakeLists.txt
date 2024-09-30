cmake_minimum_required(VERSION 3.22.1)

add_subdirectory(pl0-compiler)

include(cmake/clang-format.cmake)
include(cmake/cmake-format.cmake)

enable_testing()
add_subdirectory(test)

add_executable(hello-world main.cc)
