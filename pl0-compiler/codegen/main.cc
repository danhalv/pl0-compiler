#include "codegen/codegen.hh"
#include "lexer/lexer.hh"
#include "parser/parser.hh"

#include <fstream>
#include <iostream>

int main(void)
{
  const auto textString = std::string{
      "module myModule; begin while 1 = 2 then x := 1; end; end myModule."};
  auto text = std::vector<unsigned char>(textString.length());
  text.insert(text.begin(), textString.begin(), textString.end());

  pl0c::codegen::run(pl0c::parser::run(pl0c::lexer::run(text)));

  auto assemblyFile = std::ifstream{"program.asm"};
  if (assemblyFile.is_open())
    std::cout << assemblyFile.rdbuf();
  assemblyFile.close();

  system("as -o program.o program.asm");
  system("ld -s -o program program.o");
  system("./program");

  return 0;
}
