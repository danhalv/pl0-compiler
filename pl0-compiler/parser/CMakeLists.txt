add_library(
  parser
  parser/parser.cc
  parser/ast/program_node.cc
  parser/ast/block_node.cc
  parser/ast/const_decl_node.cc
  parser/ast/var_decl_node.cc
  parser/ast/proc_decl_node.cc
  parser/ast/assign_stmt_node.cc
  parser/ast/call_stmt_node.cc
  parser/ast/if_stmt_node.cc
  parser/ast/out_stmt_node.cc
  parser/ast/while_stmt_node.cc
  parser/ast/division_expr_node.cc
  parser/ast/id_expr_node.cc
  parser/ast/input_expr_node.cc
  parser/ast/int_expr_node.cc
  parser/ast/minus_expr_node.cc
  parser/ast/multiplication_expr_node.cc
  parser/ast/negative_expr_node.cc
  parser/ast/plus_expr_node.cc
  parser/ast/comparison_test_node.cc
  parser/ast/odd_test_node.cc)
target_include_directories(parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(parser lexer)

find_package(GTest REQUIRED)

add_executable(parser_test test/parser_test.cc)
target_link_libraries(parser_test lexer parser ${GTEST_BOTH_LIBRARIES})

gtest_discover_tests(parser_test)
