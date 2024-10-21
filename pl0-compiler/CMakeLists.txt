cmake_minimum_required(VERSION 3.22.1)

project(pl0-compiler VERSION 1.0.0)
enable_language(CXX)
enable_testing()
set(CMAKE_CXX_COMPILER "/usr/bin/clang++-15")
set(CMAKE_CXX_COMPILER_VERSION 20)
set(CMAKE_CXX_FLAGS
    "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wunused -Wconversion\
     -Wold-style-cast -Wsign-conversion -Wnarrowing")
set(CMAKE_CXX_CLANG_TIDY "/usr/bin/clang-tidy-15")

add_subdirectory(lexer)
add_subdirectory(parser)
add_subdirectory(codegen)
