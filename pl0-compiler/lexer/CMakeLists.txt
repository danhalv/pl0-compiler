add_library(lexer lexer/lexer.cc lexer/token/token.cc)
target_include_directories(lexer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

find_package(GTest REQUIRED)

add_executable(lexer_test test/lexer_test.cc)
target_link_libraries(lexer_test lexer ${GTEST_BOTH_LIBRARIES})

gtest_discover_tests(lexer_test)
