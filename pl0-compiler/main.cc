#include "analyzer/analyzer.hh"
#include "codegen/codegen.hh"
#include "lexer/lexer.hh"
#include "parser/parser.hh"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

auto printHelp() -> void
{
  std::cout
      << "Program Name: pl0-compiler\n"
      << "Description: This is a compiler for the pl0 programming"
      << " language.\n"
      << "\n"
      << "Usage: ./pl0c [options] <file>\n"
      << "\n"
      << "Options:\n"
      << "  -h, --help          Show this help message.\n"
      << "  -c, --compile       Compiles to object file.\n"
      << "  -a, --assembly      Prints x86-64 assembly in AT&T syntax for"
      << " given pl0 program file.\n"
      << "\n"
      << "Examples:\n"
      << "  ./pl0c <file>       Compiles and executes the given pl0 program "
         "file.\n"
      << "  ./pl0c -c <file>    Only compiles the given pl0 program file.\n"
      << "  ./pl0c -a <file>    Prints x86-64 assembly in AT&T syntax for"
      << " given pl0 program file.\n"
      << std::endl;
}

auto printAssembly(const std::string &fileName) -> void
{
  auto is = std::ifstream{fileName};

  if (!is)
  {
    std::cout << "Can't read assembly file." << std::endl;
    exit(1);
  }

  auto line = std::string{};
  while (std::getline(is, line))
  {
    std::cout << line << "\n";
  }
  std::cout << std::endl;
}

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
    printHelp();
    return 1;
  }

  bool shouldOnlyCompile = false;
  bool shouldPrintAssembly = false;

  // handle arguments
  for (int i = 0; i < argc; ++i)
  {
    const auto arg = std::string{argv[i]};

    if ("-c" == arg || "--compile" == arg)
    {
      shouldOnlyCompile = true;
    }
    else if ("-h" == arg || "--help" == arg)
    {
      printHelp();
      return 0;
    }
    else if ("-a" == arg || "--assembly" == arg)
    {
      shouldPrintAssembly = true;
    }
  }

  // execute compiler steps to generate x86-64 assembly
  // assumes file is last argument
  const auto ast = pl0c::parser::run(
      pl0c::lexer::run(readFile(std::string{argv[argc - 1]})));
  pl0c::analyzer::run(ast);
  pl0c::codegen::run(ast);

  if (shouldPrintAssembly)
  {
    printAssembly("program.asm");
    return 0;
  }

  // translate assembly to machine code
  system("as -o program.o program.asm");

  // create executable and execute program
  if (!shouldOnlyCompile)
  {
    system("ld -o program -dynamic-linker /lib64/ld-linux-x86-64.so.2 -lc "
           "program.o");
    system("./program");
  }

  return 0;
}
