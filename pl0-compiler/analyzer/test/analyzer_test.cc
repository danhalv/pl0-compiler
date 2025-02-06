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

TEST(AnalyzerGlobalTest, constRedeclaration)
{
  const auto textString = std::string{
      "module myModule; const x : int = 0, x : int = 1; begin end myModule."};

  ASSERT_DEATH(pl0c::analyzer::run(
                   pl0c::parser::run(pl0c::lexer::run(createText(textString)))),
               "");
}

TEST(AnalyzerGlobalTest, procedureRedeclaration)
{
  const auto textString =
      std::string{"module myModule; procedure foo(); begin end foo; procedure "
                  "foo(); begin end foo; begin end myModule."};

  ASSERT_DEATH(pl0c::analyzer::run(
                   pl0c::parser::run(pl0c::lexer::run(createText(textString)))),
               "");
}

TEST(AnalyzerGlobalTest, varRedeclaresProcedureId)
{
  const auto textString =
      std::string{"module myModule; procedure foo(); begin end foo; var foo : "
                  "int; begin end myModule."};

  ASSERT_DEATH(pl0c::analyzer::run(
                   pl0c::parser::run(pl0c::lexer::run(createText(textString)))),
               "");
}

TEST(AnalyzerGlobalTest, varRedeclaration)
{
  const auto textString =
      std::string{"module myModule; var x : int, x : int; begin end myModule."};

  ASSERT_DEATH(pl0c::analyzer::run(
                   pl0c::parser::run(pl0c::lexer::run(createText(textString)))),
               "");
}

TEST(AnalyzerGlobalTest, varRedeclaresConstId)
{
  const auto textString = std::string{
      "module myModule; const x : int = 0; var x : int; begin end myModule."};

  ASSERT_DEATH(pl0c::analyzer::run(
                   pl0c::parser::run(pl0c::lexer::run(createText(textString)))),
               "");
}

TEST(AnalyzerGlobalTest, assignToUndeclaredVariableGlobal)
{
  const auto textString =
      std::string{"module myModule; begin x := 1; end myModule."};

  ASSERT_DEATH(pl0c::analyzer::run(
                   pl0c::parser::run(pl0c::lexer::run(createText(textString)))),
               "");
}

TEST(AnalyzerGlobalTest, assignToUndeclaredVariableLocal)
{
  const auto textString = std::string{"module myModule; procedure foo(); begin "
                                      "x := 1; end foo; begin end myModule."};

  ASSERT_DEATH(pl0c::analyzer::run(
                   pl0c::parser::run(pl0c::lexer::run(createText(textString)))),
               "");
}

TEST(AnalyzerGlobalTest, callToUndeclaredFunction)
{
  const auto textString =
      std::string{"module myModule; begin foo(); end myModule."};

  ASSERT_DEATH(pl0c::analyzer::run(
                   pl0c::parser::run(pl0c::lexer::run(createText(textString)))),
               "");
}

TEST(AnalyzerGlobalTest, addUndeclaredId)
{
  const auto textString =
      std::string{"module myModule; begin output := x + 1; end myModule."};

  ASSERT_DEATH(pl0c::analyzer::run(
                   pl0c::parser::run(pl0c::lexer::run(createText(textString)))),
               "");
}

TEST(AnalyzerGlobalTest, divisionExprUndeclaredId)
{
  const auto textString =
      std::string{"module myModule; begin output := x / 2; end myModule."};

  ASSERT_DEATH(pl0c::analyzer::run(
                   pl0c::parser::run(pl0c::lexer::run(createText(textString)))),
               "");
}

TEST(AnalyzerGlobalTest, negativeExprUndeclaredId)
{
  const auto textString =
      std::string{"module myModule; begin output := -y; end myModule."};

  ASSERT_DEATH(pl0c::analyzer::run(
                   pl0c::parser::run(pl0c::lexer::run(createText(textString)))),
               "");
}

TEST(AnalyzerLocalTest, subtractUndeclaredId)
{
  const auto textString =
      std::string{"module myModule; procedure foo (); begin "
                  "output := x - 2; end foo; begin end myModule."};

  ASSERT_DEATH(pl0c::analyzer::run(
                   pl0c::parser::run(pl0c::lexer::run(createText(textString)))),
               "");
}

TEST(AnalyzerLocalTest, multiplicationExprUndeclaredId)
{
  const auto textString =
      std::string{"module myModule; procedure foo (); begin output := 2 * x; "
                  "end foo; begin end myModule."};

  ASSERT_DEATH(pl0c::analyzer::run(
                   pl0c::parser::run(pl0c::lexer::run(createText(textString)))),
               "");
}

TEST(AnalyzerLocalTest, ifUndeclaredId)
{
  const auto textString = std::string{
      "module myModule; procedure myProcedure (); begin if odd 1 then x := 1; "
      "end; end myProcedure; begin myProcedure(); end myModule."};

  ASSERT_DEATH(pl0c::analyzer::run(
                   pl0c::parser::run(pl0c::lexer::run(createText(textString)))),
               "");
}

TEST(AnalyzerLocalTest, whileUndeclaredId)
{
  const auto textString = std::string{
      "module myModule; procedure myProcedure (); begin while odd 1 do "
      "output := x; end; end myProcedure; begin myProcedure(); end myModule."};

  ASSERT_DEATH(pl0c::analyzer::run(
                   pl0c::parser::run(pl0c::lexer::run(createText(textString)))),
               "");
}

TEST(AnalyzerLocalTest, varRedeclaresProcedureId)
{
  const auto textString =
      std::string{"module myModule; procedure foo(); var foo : int; begin end "
                  "foo; begin end myModule."};

  ASSERT_DEATH(pl0c::analyzer::run(
                   pl0c::parser::run(pl0c::lexer::run(createText(textString)))),
               "");
}
