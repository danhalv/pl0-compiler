#include "codegen/codegen.hh"

#include "parser/ast/block_node.hh"
#include "parser/ast/expr_node.hh"
#include "parser/ast/int_expr_node.hh"
#include "parser/ast/out_stmt_node.hh"
#include "parser/ast/program_node.hh"
#include "parser/ast/stmt_node.hh"

#include <cassert>
#include <fstream>
#include <memory>

namespace pl0c
{
namespace codegen
{

auto cgen(const std::shared_ptr<parser::ExprNode> exprNode,
          std::ofstream &assemblyFile) -> void
{
  switch (exprNode->getType())
  {
  case parser::ExprNodeType::INT_EXPR: {
    const auto intExprNode =
        std::dynamic_pointer_cast<parser::IntExprNode>(exprNode);

    assemblyFile << "push $" << std::to_string(intExprNode->getValue()) << "\n";
    break;
  }
  default: {
    const auto errMsg =
        std::string{"codegen error: unsupported expression type."};
    assert(errMsg.c_str() && false);
  }
  }
}

auto cgen(const std::shared_ptr<parser::StmtNode> stmtNode,
          std::ofstream &assemblyFile) -> void
{
  switch (stmtNode->getType())
  {
  case parser::StmtNodeType::OUT_STMT: {
    const auto exprNode =
        std::dynamic_pointer_cast<parser::OutStmtNode>(stmtNode)->getExprNode();
    cgen(exprNode, assemblyFile);

    assemblyFile << "pop %rsi\n"
                 << "lea format, %rdi\n"
                 << "call printf\n";
    break;
  }
  default: {
    const auto errMsg =
        std::string{"codegen error: unsupported statement type."};
    assert(errMsg.c_str() && false);
  }
  }
}

auto cgen(const parser::BlockNode &blockNode, std::ofstream &assemblyFile)
    -> void
{
  assemblyFile << ".section .text\n"
               << ".global _start, printf, exit\n"
               << "_start:\n";

  for (auto statement : blockNode.getStatements())
  {
    cgen(statement, assemblyFile);
  }
}

auto run(const parser::ProgramNode &programNode) -> void
{
  auto assemblyFile = std::ofstream{"program.asm"};

  assemblyFile << ".code64\n"
               << ".section .rodata\n"
               << "format: .asciz \"%d\"\n";

  cgen(programNode.getBlockNode(), assemblyFile);

  assemblyFile << "push $1\n"
               << "call exit" << std::endl;

  assemblyFile.close();
}

}; // namespace codegen
}; // namespace pl0c
