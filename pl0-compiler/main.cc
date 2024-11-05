#include "codegen/codegen.hh"
#include "lexer/lexer.hh"
#include "parser/parser.hh"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

auto readFile(const std::string &fileName) -> std::vector<unsigned char>
{
  auto is = std::ifstream{fileName};

  if (!is)
  {
    std::cout << "Can't read file: " << fileName;
    exit(1);
  }

  auto text = std::vector<unsigned char>{};

  auto line = std::string{};
  while (std::getline(is, line))
  {
    for (const auto character : line)
      text.push_back(static_cast<unsigned char>(character));
    text.push_back('\n');
  }

  return text;
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cout << "No input file." << std::endl;
    return 1;
  }

  pl0c::codegen::run(
      pl0c::parser::run(pl0c::lexer::run(readFile(std::string{argv[1]}))));

  system("as -o program.o program.asm");
  system("ld -o program -dynamic-linker /lib64/ld-linux-x86-64.so.2 -lc "
         "program.o");
  system("./program");

  return 0;
}
