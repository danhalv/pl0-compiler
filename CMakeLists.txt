cmake_minimum_required(VERSION 3.22.1)

project(pl0-compiler VERSION 1.0.0)
enable_language(CXX)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++-18")
set(CMAKE_CXX_COMPILER_VERSION 20)
set(CMAKE_CXX_CLANG_TIDY "/usr/bin/clang-tidy-18")

include(cmake/clang-format.cmake)
include(cmake/cmake-format.cmake)

add_executable(hello-world main.cc)
