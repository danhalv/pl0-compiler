#include "codegen/codegen.hh"
#include "lexer/lexer.hh"
#include "parser/parser.hh"

#include <fstream>
#include <gtest/gtest.h>
#include <string>
#include <vector>

auto createText(const std::string &textString) -> std::vector<unsigned char>
{
  auto text = std::vector<unsigned char>(textString.length());
  text.insert(text.begin(), textString.begin(), textString.end());

  return text;
}

auto executeAssemblyFileAndGetOutput() -> std::string
{
  const auto outputFileName = std::string{"out.txt"};

  system("as -o program.o program.asm");
  system("ld -o program -dynamic-linker /lib64/ld-linux-x86-64.so.2 -lc "
         "program.o");
  system(std::string{"./program > " + outputFileName}.c_str());

  std::string fileContent;
  std::getline(std::ifstream("out.txt"), fileContent, '\0');

  return fileContent;
}

TEST(CodegenArithmeticTest, addTwoIntegers)
{
  const auto textString =
      std::string{"module myModule; begin output := 10 + 15; end myModule."};

  pl0c::codegen::run(
      pl0c::parser::run(pl0c::lexer::run(createText(textString))));

  const auto programOutput = executeAssemblyFileAndGetOutput();

  ASSERT_EQ(programOutput, "25\n");
}

TEST(CodegenArithmeticTest, subtractTwoIntegers)
{
  const auto textString =
      std::string{"module myModule; begin output := 5 - 3; end myModule."};

  pl0c::codegen::run(
      pl0c::parser::run(pl0c::lexer::run(createText(textString))));

  const auto programOutput = executeAssemblyFileAndGetOutput();

  ASSERT_EQ(programOutput, "2\n");
}

TEST(CodegenArithmeticTest, divideTwoIntegers)
{
  const auto textString =
      std::string{"module myModule; begin output := 6 / 2; end myModule."};

  pl0c::codegen::run(
      pl0c::parser::run(pl0c::lexer::run(createText(textString))));

  const auto programOutput = executeAssemblyFileAndGetOutput();

  ASSERT_EQ(programOutput, "3\n");
}

TEST(CodegenArithmeticTest, multiplyTwoIntegers)
{
  const auto textString =
      std::string{"module myModule; begin output := 4 * 3; end myModule."};

  pl0c::codegen::run(
      pl0c::parser::run(pl0c::lexer::run(createText(textString))));

  const auto programOutput = executeAssemblyFileAndGetOutput();

  ASSERT_EQ(programOutput, "12\n");
}

TEST(CodegenAssignTest, assignDeclaredVariable)
{
  const auto textString = std::string{
      "module myModule; var i : int; begin i := 5; output := i; end myModule."};

  pl0c::codegen::run(
      pl0c::parser::run(pl0c::lexer::run(createText(textString))));

  const auto programOutput = executeAssemblyFileAndGetOutput();

  ASSERT_EQ(programOutput, "5\n");
}

TEST(CodegenConstDeclTest, outputAliasedConstDeclaration)
{
  const auto textString =
      std::string{"module myModule; const i : int = 11; const x : int = i; "
                  "begin output := x; end myModule."};

  pl0c::codegen::run(
      pl0c::parser::run(pl0c::lexer::run(createText(textString))));

  const auto programOutput = executeAssemblyFileAndGetOutput();

  ASSERT_EQ(programOutput, "11\n");
}

TEST(CodegenProcDeclTest, nestedProcedures)
{
  const auto textString = std::string{"module myModule;"
                                      "procedure foo(x : int, y : int);"
                                      "const z : int = x;"
                                      "procedure bar(a : int);"
                                      "var x : int;"
                                      "begin x := a; output := x;"
                                      "end bar;"
                                      "begin output := z; bar(y);"
                                      "end foo;"
                                      "begin foo(2, 7); end myModule."};

  pl0c::codegen::run(
      pl0c::parser::run(pl0c::lexer::run(createText(textString))));

  const auto programOutput = executeAssemblyFileAndGetOutput();

  ASSERT_EQ(programOutput, "2\n7\n");
}

TEST(CodegenIfStmtTest, outputOnTrue)
{
  const auto textString = std::string{"module myModule;"
                                      "const x : int = 1;"
                                      "begin if x < 2 then output := 1; end;"
                                      "if 0 < 2 then output := 2; end;"
                                      "end myModule."};

  pl0c::codegen::run(
      pl0c::parser::run(pl0c::lexer::run(createText(textString))));

  const auto programOutput = executeAssemblyFileAndGetOutput();

  ASSERT_EQ(programOutput, "1\n2\n");
}

TEST(CodegenIfStmtTest, ifGreaterThanThenOutput)
{
  const auto textString = std::string{"module myModule;"
                                      "begin if 2 > 1 then output := 1; end;"
                                      "end myModule."};

  pl0c::codegen::run(
      pl0c::parser::run(pl0c::lexer::run(createText(textString))));

  const auto programOutput = executeAssemblyFileAndGetOutput();

  ASSERT_EQ(programOutput, "1\n");
}

TEST(CodegenIfStmtTest, ifGreaterThanOrEqualThenOutput1)
{
  const auto textString = std::string{"module myModule;"
                                      "begin if 2 >= 1 then output := 1; end;"
                                      "end myModule."};

  pl0c::codegen::run(
      pl0c::parser::run(pl0c::lexer::run(createText(textString))));

  const auto programOutput = executeAssemblyFileAndGetOutput();

  ASSERT_EQ(programOutput, "1\n");
}

TEST(CodegenIfStmtTest, ifGreaterThanOrEqualThenOutput2)
{
  const auto textString = std::string{"module myModule;"
                                      "begin if 2 >= 2 then output := 1; end;"
                                      "end myModule."};

  pl0c::codegen::run(
      pl0c::parser::run(pl0c::lexer::run(createText(textString))));

  const auto programOutput = executeAssemblyFileAndGetOutput();

  ASSERT_EQ(programOutput, "1\n");
}

TEST(CodegenIfStmtTest, ifEqualThenOutput)
{
  const auto textString = std::string{"module myModule;"
                                      "begin if 2 = 2 then output := 1; end;"
                                      "end myModule."};

  pl0c::codegen::run(
      pl0c::parser::run(pl0c::lexer::run(createText(textString))));

  const auto programOutput = executeAssemblyFileAndGetOutput();

  ASSERT_EQ(programOutput, "1\n");
}

TEST(CodegenIfStmtTest, ifNotEqualThenOutput)
{
  const auto textString = std::string{"module myModule;"
                                      "begin if 1 <> 2 then output := 1; end;"
                                      "end myModule."};

  pl0c::codegen::run(
      pl0c::parser::run(pl0c::lexer::run(createText(textString))));

  const auto programOutput = executeAssemblyFileAndGetOutput();

  ASSERT_EQ(programOutput, "1\n");
}

TEST(CodegenIfStmtTest, ifLessThanOrEqualThenOutput1)
{
  const auto textString = std::string{"module myModule;"
                                      "begin if 1 <= 2 then output := 1; end;"
                                      "end myModule."};

  pl0c::codegen::run(
      pl0c::parser::run(pl0c::lexer::run(createText(textString))));

  const auto programOutput = executeAssemblyFileAndGetOutput();

  ASSERT_EQ(programOutput, "1\n");
}

TEST(CodegenIfStmtTest, ifLessThanOrEqualThenOutput2)
{
  const auto textString = std::string{"module myModule;"
                                      "begin if 2 <= 2 then output := 1; end;"
                                      "end myModule."};

  pl0c::codegen::run(
      pl0c::parser::run(pl0c::lexer::run(createText(textString))));

  const auto programOutput = executeAssemblyFileAndGetOutput();

  ASSERT_EQ(programOutput, "1\n");
}

TEST(CodegenIfStmtTest, noOutputOnFalse)
{
  const auto textString = std::string{"module myModule;"
                                      "const x : int = 1;"
                                      "begin if 2 < x then output := 1; end;"
                                      "end myModule."};

  pl0c::codegen::run(
      pl0c::parser::run(pl0c::lexer::run(createText(textString))));

  const auto programOutput = executeAssemblyFileAndGetOutput();

  ASSERT_EQ(programOutput, "");
}
