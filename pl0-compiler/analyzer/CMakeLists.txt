add_library(analyzer analyzer/analyzer.cc)
target_include_directories(analyzer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(analyzer parser)

find_package(GTest REQUIRED)

add_executable(analyzer_test test/analyzer_test.cc)
target_link_libraries(analyzer_test lexer parser analyzer
                      ${GTEST_BOTH_LIBRARIES})

gtest_discover_tests(analyzer_test)
