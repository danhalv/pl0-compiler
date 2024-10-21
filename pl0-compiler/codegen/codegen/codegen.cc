#include "codegen/codegen.hh"
#include <fstream>

namespace pl0c
{
namespace codegen
{

auto visit(const parser::BlockNode &blockNode, std::ofstream &assemblyFile)
    -> void
{
  assemblyFile << ".code64\n"
                  ".section .rodata\n"
                  "msg: .ascii \"Hello, World!\"\n"
                  ".set msglen, (. - msg)\n"
                  "\n"
                  ".section .text\n"
                  ".global _start\n"
                  "_start:\n"
                  "  mov $1, %rax\n"
                  "  mov $1, %rdi\n"
                  "  lea msg, %rsi\n"
                  "  mov $msglen, %rdx\n"
                  "  syscall\n"
                  "\n"
                  "  mov $60, %rax\n"
                  "  mov $0, %rdi\n"
                  "  syscall\n"
               << std::endl;
}

auto run(const parser::ProgramNode &programNode) -> void
{
  auto assemblyFile = std::ofstream{"program.asm"};

  visit(programNode.getBlockNode(), assemblyFile);

  assemblyFile.close();
}

}; // namespace codegen
}; // namespace pl0c
