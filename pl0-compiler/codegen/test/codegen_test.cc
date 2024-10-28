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

TEST(CodegenAdditionTest, addTwoIntegers)
{
  const auto textString =
      std::string{"module myModule; begin output := 10 + 15; end myModule."};

  pl0c::codegen::run(
      pl0c::parser::run(pl0c::lexer::run(createText(textString))));

  const auto programOutput = executeAssemblyFileAndGetOutput();

  ASSERT_EQ(programOutput, "25");
}
