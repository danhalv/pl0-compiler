#include "codegen/codegen.hh"

#include "codegen/scratch_register_manager.hh"
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

struct CgenContext
{
  CgenContext() = default;

  std::ofstream asmFile{"program.asm"};
  ScratchRegisterManager regMgr;
};

auto cgen(const std::shared_ptr<parser::ExprNode> exprNode, CgenContext &ctx)
    -> ScratchRegister
{
  const auto reg = ctx.regMgr.alloc();

  switch (exprNode->getType())
  {
  case parser::ExprNodeType::INT_EXPR: {
    const auto intExprNode =
        std::dynamic_pointer_cast<parser::IntExprNode>(exprNode);
    const auto value = std::to_string(intExprNode->getValue());

    if (reg == ScratchRegister::NONE)
    {
      ctx.asmFile << "push $" << value << "\n";
    }
    else
    {
      ctx.asmFile << "mov $" << value << ", "
                  << scratchRegisterStringMap.at(reg) << "\n";
    }

    break;
  }
  default: {
    const auto errMsg =
        std::string{"codegen error: unsupported expression type."};
    assert(errMsg.c_str() && false);
  }
  }

  return reg;
}

auto cgen(const std::shared_ptr<parser::StmtNode> stmtNode, CgenContext &ctx)
    -> void
{
  switch (stmtNode->getType())
  {
  case parser::StmtNodeType::OUT_STMT: {
    const auto exprNode =
        std::dynamic_pointer_cast<parser::OutStmtNode>(stmtNode)->getExprNode();

    auto reg = cgen(exprNode, ctx);

    if (reg == ScratchRegister::NONE)
    {
      ctx.asmFile << "pop %rsi\n"
                  << "lea format, %rdi\n"
                  << "call printf\n";
    }
    else
    {
      ctx.asmFile << "mov " << scratchRegisterStringMap.at(reg) << ", %rsi\n"
                  << "lea format, %rdi\n"
                  << "call printf\n";
      ctx.regMgr.free(reg);
    }

    break;
  }
  default: {
    const auto errMsg =
        std::string{"codegen error: unsupported statement type."};
    assert(errMsg.c_str() && false);
  }
  }
}

auto cgen(const parser::BlockNode &blockNode, CgenContext &ctx) -> void
{
  ctx.asmFile << ".section .text\n"
              << ".global _start, printf, exit\n"
              << "_start:\n";

  for (auto statement : blockNode.getStatements())
  {
    cgen(statement, ctx);
  }
}

auto run(const parser::ProgramNode &programNode) -> void
{
  auto ctx = CgenContext{};

  ctx.asmFile << ".code64\n"
              << ".section .rodata\n"
              << "format: .asciz \"%d\"\n";

  cgen(programNode.getBlockNode(), ctx);

  ctx.asmFile << "push $1\n"
              << "call exit" << std::endl;

  ctx.asmFile.close();
}

}; // namespace codegen
}; // namespace pl0c
