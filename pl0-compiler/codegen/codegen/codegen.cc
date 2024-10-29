#include "codegen/codegen.hh"

#include "codegen/scratch_register_manager.hh"
#include "parser/ast/assign_stmt_node.hh"
#include "parser/ast/block_node.hh"
#include "parser/ast/decl_node.hh"
#include "parser/ast/expr_node.hh"
#include "parser/ast/id_expr_node.hh"
#include "parser/ast/int_expr_node.hh"
#include "parser/ast/out_stmt_node.hh"
#include "parser/ast/plus_expr_node.hh"
#include "parser/ast/program_node.hh"
#include "parser/ast/stmt_node.hh"
#include "parser/ast/var_decl_node.hh"

#include <cassert>
#include <cstdint>
#include <fstream>
#include <memory>
#include <sstream>

namespace pl0c
{
namespace codegen
{

struct CgenContext
{
  CgenContext() : dataSection{}, roDataSection{}, textSection{}, regMgr{}
  {
    dataSection << ".section .data\n";
    roDataSection << ".section .rodata\n";
    textSection << ".section .text\n";
  }

  std::stringstream dataSection;
  std::stringstream roDataSection;
  std::stringstream textSection;
  ScratchRegisterManager regMgr;
};

auto cgen(const std::shared_ptr<parser::ExprNode> exprNode, CgenContext &ctx)
    -> ScratchRegister
{
  switch (exprNode->getType())
  {
  case parser::ExprNodeType::ID_EXPR: {
    const auto idExprNode =
        std::dynamic_pointer_cast<parser::IdExprNode>(exprNode);

    const auto reg = ctx.regMgr.alloc();
    if (reg == ScratchRegister::NONE)
    {
      ctx.textSection << "push " << idExprNode->getId() << "\n";
    }
    else
    {
      ctx.textSection << "mov " << idExprNode->getId() << ", "
                      << scratchRegisterStringMap.at(reg) << "\n";
    }

    return reg;
  }
  case parser::ExprNodeType::INT_EXPR: {
    const auto intExprNode =
        std::dynamic_pointer_cast<parser::IntExprNode>(exprNode);
    const auto value = std::to_string(intExprNode->getValue());

    const auto reg = ctx.regMgr.alloc();
    if (reg == ScratchRegister::NONE)
    {
      ctx.textSection << "push $" << value << "\n";
    }
    else
    {
      ctx.textSection << "mov $" << value << ", "
                      << scratchRegisterStringMap.at(reg) << "\n";
    }

    return reg;
  }
  case parser::ExprNodeType::PLUS_EXPR: {
    const auto plusExprNode =
        std::dynamic_pointer_cast<parser::PlusExprNode>(exprNode);

    auto leftExprReg = cgen(plusExprNode->getLhsExpr(), ctx);
    auto rightExprReg = cgen(plusExprNode->getRhsExpr(), ctx);

    if (leftExprReg == ScratchRegister::NONE &&
        rightExprReg == ScratchRegister::NONE)
    {
      ctx.textSection << "pop %rax\n"
                      << "add %rsp, %rax\n"
                      << "pop\n"
                      << "push %rax\n";
      return ScratchRegister::NONE;
    }

    if (leftExprReg == ScratchRegister::NONE)
    {
      ctx.textSection << "pop %rax\n"
                      << "add " << scratchRegisterStringMap.at(rightExprReg)
                      << ", %rax\n"
                      << "\n";
      return rightExprReg;
    }

    if (rightExprReg == ScratchRegister::NONE)
    {
      ctx.textSection << "pop %rax\n"
                      << "add " << scratchRegisterStringMap.at(leftExprReg)
                      << ", %rax\n"
                      << "\n";
      return leftExprReg;
    }

    ctx.textSection << "add " << scratchRegisterStringMap.at(leftExprReg)
                    << ", " << scratchRegisterStringMap.at(rightExprReg)
                    << "\n";
    ctx.regMgr.free(leftExprReg);
    return rightExprReg;
  }
  default: {
    const auto errMsg =
        std::string{"codegen error: unsupported expression type."};
    assert(errMsg.c_str() && false);
  }
  }

  return ScratchRegister::NONE;
}

auto cgen(const std::shared_ptr<parser::StmtNode> stmtNode, CgenContext &ctx)
    -> void
{
  switch (stmtNode->getType())
  {
  case parser::StmtNodeType::ASSIGN_STMT: {
    const auto assignStmtNode =
        std::dynamic_pointer_cast<parser::AssignStmtNode>(stmtNode);
    ;

    const auto reg = cgen(assignStmtNode->getExprNode(), ctx);

    if (reg == ScratchRegister::NONE)
    {
      ctx.textSection << "pop " << assignStmtNode->getLValueId() << "\n";
    }
    else
    {
      ctx.textSection << "mov " << scratchRegisterStringMap.at(reg) << ", "
                      << assignStmtNode->getLValueId() << "\n";
      ctx.regMgr.free(reg);
    }

    break;
  }
  case parser::StmtNodeType::OUT_STMT: {
    const auto exprNode =
        std::dynamic_pointer_cast<parser::OutStmtNode>(stmtNode)->getExprNode();

    auto reg = cgen(exprNode, ctx);

    if (reg == ScratchRegister::NONE)
    {
      ctx.textSection << "pop %rsi\n"
                      << "lea format, %rdi\n"
                      << "call printf\n";
    }
    else
    {
      ctx.textSection << "mov " << scratchRegisterStringMap.at(reg)
                      << ", %rsi\n"
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

auto cgen(std::shared_ptr<parser::DeclNode> declNode, CgenContext &ctx) -> void
{
  switch (declNode->getType())
  {
  case parser::DeclNodeType::VAR_DECL: {
    const auto varDeclNode =
        std::dynamic_pointer_cast<parser::VarDeclNode>(declNode);

    ctx.dataSection << varDeclNode->getDeclId() << ": .value 0\n";

    break;
  }
  default: {
    const auto errMsg =
        std::string{"codegen error: unsupported declaration type."};
    assert(errMsg.c_str() && false);
  }
  }
}

auto cgen(const parser::BlockNode &blockNode, CgenContext &ctx) -> void
{
  for (auto declaration : blockNode.getDeclarations())
  {
    cgen(declaration, ctx);
  }

  ctx.textSection << "_start:\n";

  for (auto statement : blockNode.getStatements())
  {
    cgen(statement, ctx);
  }
}

auto run(const parser::ProgramNode &programNode) -> void
{
  std::ofstream asmFile{"program.asm"};
  asmFile << ".code64\n";

  auto ctx = CgenContext{};

  ctx.roDataSection << "format: .asciz \"%d\"\n";

  cgen(programNode.getBlockNode(), ctx);

  ctx.textSection << "push $1\n"
                  << "call exit" << std::endl;

  asmFile << ctx.roDataSection.rdbuf() << ctx.dataSection.rdbuf()
          << ".global _start, printf, exit\n"
          << ctx.textSection.rdbuf();
  asmFile.close();
}

}; // namespace codegen
}; // namespace pl0c
