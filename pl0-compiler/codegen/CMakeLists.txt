add_library(codegen codegen/codegen.cc)
target_include_directories(codegen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(codegen parser)

add_executable(main main.cc)
target_link_libraries(main lexer parser codegen)
