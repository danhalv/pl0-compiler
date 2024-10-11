add_library(
  parser
  parser/parser.cc
  parser/ast/program_node.cc
  parser/ast/block_node.cc
  parser/ast/const_decl_node.cc
  parser/ast/var_decl_node.cc
  parser/ast/proc_decl_node.cc
  parser/ast/assign_stmt_node.cc)
target_include_directories(parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(parser lexer)

find_package(GTest REQUIRED)

add_executable(parser_test test/parser_test.cc)
target_link_libraries(parser_test lexer parser ${GTEST_BOTH_LIBRARIES})

gtest_discover_tests(parser_test)
