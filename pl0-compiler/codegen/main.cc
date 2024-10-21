#include "codegen/codegen.hh"
#include "lexer/lexer.hh"
#include "parser/parser.hh"

#include <fstream>
#include <iostream>

int main(void)
{
  const auto textString =
      std::string{"module myModule; begin output := 14; end myModule."};
  auto text = std::vector<unsigned char>(textString.length());
  text.insert(text.begin(), textString.begin(), textString.end());

  pl0c::codegen::run(pl0c::parser::run(pl0c::lexer::run(text)));

  auto assemblyFile = std::ifstream{"program.asm"};
  if (assemblyFile.is_open())
    std::cout << assemblyFile.rdbuf();
  std::cout << std::endl;
  assemblyFile.close();

  system("as -o program.o program.asm");
  system("ld -o program -dynamic-linker /lib64/ld-linux-x86-64.so.2 -lc "
         "program.o");
  system("./program");

  return 0;
}
