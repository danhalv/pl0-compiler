file(GLOB_RECURSE SOURCE_FILES *.cc *.hh)

add_custom_target(
  clang-format-check ALL
  COMMAND clang-format-15 --dry-run --Werror ${SOURCE_FILES}
  DEPENDS .clang-format)
