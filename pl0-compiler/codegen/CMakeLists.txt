add_library(codegen codegen/codegen.cc codegen/scratch_register_manager.cc
                    codegen/label_manager.cc)
target_include_directories(codegen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(codegen parser)

find_package(GTest REQUIRED)

add_executable(codegen_test test/codegen_test.cc)
target_link_libraries(codegen_test lexer parser codegen ${GTEST_BOTH_LIBRARIES})

gtest_discover_tests(codegen_test)

# add_executable(main main.cc) target_link_libraries(main lexer parser codegen)
