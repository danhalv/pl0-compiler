#include "analyzer/analyzer.hh"
#include "lexer/lexer.hh"
#include "parser/parser.hh"

#include <gtest/gtest.h>
#include <string>
#include <vector>

auto createText(const std::string &textString) -> std::vector<unsigned char>
{
  auto text = std::vector<unsigned char>(textString.length());
  text.insert(text.begin(), textString.begin(), textString.end());

  return text;
}

TEST(AnalyzerGlobalVarTest, constRedeclaration)
{
  const auto textString = std::string{
      "module myModule; const x : int = 0, x : int = 1; begin end myModule."};

  ASSERT_DEATH(pl0c::analyzer::run(
                   pl0c::parser::run(pl0c::lexer::run(createText(textString)))),
               "");
}

TEST(AnalyzerGlobalVarTest, procedureRedeclaration)
{
  const auto textString =
      std::string{"module myModule; procedure foo(); begin end foo; procedure "
                  "foo(); begin end foo; begin end myModule."};

  ASSERT_DEATH(pl0c::analyzer::run(
                   pl0c::parser::run(pl0c::lexer::run(createText(textString)))),
               "");
}

TEST(AnalyzerGlobalVarTest, varRedeclaresProcedureId)
{
  const auto textString =
      std::string{"module myModule; procedure foo(); begin end foo; var foo : "
                  "int; begin end myModule."};

  ASSERT_DEATH(pl0c::analyzer::run(
                   pl0c::parser::run(pl0c::lexer::run(createText(textString)))),
               "");
}

TEST(AnalyzerGlobalVarTest, varRedeclaration)
{
  const auto textString =
      std::string{"module myModule; var x : int, x : int; begin end myModule."};

  ASSERT_DEATH(pl0c::analyzer::run(
                   pl0c::parser::run(pl0c::lexer::run(createText(textString)))),
               "");
}

TEST(AnalyzerGlobalVarTest, varRedeclaresConstId)
{
  const auto textString = std::string{
      "module myModule; const x : int = 0; var x : int; begin end myModule."};

  ASSERT_DEATH(pl0c::analyzer::run(
                   pl0c::parser::run(pl0c::lexer::run(createText(textString)))),
               "");
}
