enable_testing()
find_package(GTest REQUIRED)

add_executable(gtest_example gtest_example.cc)
target_link_libraries(gtest_example ${GTEST_BOTH_LIBRARIES})

gtest_discover_tests(gtest_example)
