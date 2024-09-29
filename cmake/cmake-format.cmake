file(GLOB_RECURSE CMAKE_FILES CMakeLists.txt cmake/*.cmake)

add_custom_target(cmake-format-check ALL COMMAND cmake-format --check
                                                 ${CMAKE_FILES})
