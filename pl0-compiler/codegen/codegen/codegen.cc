#include "codegen/codegen.hh"

#include "codegen/scratch_register_manager.hh"
#include "lexer/token/token_type.hh"
#include "parser/ast/assign_stmt_node.hh"
#include "parser/ast/block_node.hh"
#include "parser/ast/call_stmt_node.hh"
#include "parser/ast/const_decl_node.hh"
#include "parser/ast/decl_node.hh"
#include "parser/ast/expr_node.hh"
#include "parser/ast/id_expr_node.hh"
#include "parser/ast/int_expr_node.hh"
#include "parser/ast/out_stmt_node.hh"
#include "parser/ast/plus_expr_node.hh"
#include "parser/ast/proc_decl_node.hh"
#include "parser/ast/program_node.hh"
#include "parser/ast/stmt_node.hh"
#include "parser/ast/var_decl_node.hh"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <memory>
#include <sstream>
#include <utility>

namespace pl0c
{
namespace codegen
{

struct CgenContext
{
  CgenContext()
      : isProcedureScope{false}, arguments{}, dataSection{}, roDataSection{},
        textSection{}, regMgr{}
  {
    dataSection << ".section .data\n";
    roDataSection << ".section .rodata\n";
    globalSection << ".global _start, printf, exit";
  }

  auto isLocalVariable(const std::string &variableId) const -> bool
  {
    return (this->isProcedureScope &&
            (std::find(this->localVariables.begin(), this->localVariables.end(),
                       variableId) != this->localVariables.end()));
  }

  auto isArgument(const std::string &argumentId) const -> bool
  {
    if (!this->isProcedureScope)
      return false;

    for (const auto &argument : this->arguments)
    {
      if (argumentId == argument.first)
        return true;
    }

    return false;
  }

  auto getLocalVariableStackOffset(const std::string &variableId) const
      -> std::size_t
  {
    for (std::size_t i = 0; i < this->localVariables.size(); ++i)
    {
      if (variableId == this->localVariables.at(i))
      {
        const auto bytes = 8;
        return (i + 1) * bytes;
      }
    }
    return 0;
  }

  auto getArgumentStackOffset(const std::string &argumentId) const
      -> std::size_t
  {
    const auto numArguments = this->arguments.size();
    for (std::size_t i = 0; i < numArguments; ++i)
    {
      if (argumentId == this->arguments.at(i).first)
      {
        const auto bytes = 8;
        const auto reverseIndex = (numArguments - i - 1);
        return (reverseIndex + 2) * bytes;
      }
    }
    return 0;
  }

  bool isProcedureScope;
  std::vector<std::pair<std::string, std::shared_ptr<lexer::Token>>> arguments;
  std::vector<std::string> localVariables;
  std::stringstream dataSection;
  std::stringstream roDataSection;
  std::stringstream textSection;
  std::stringstream globalSection;
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
    const auto exprId = idExprNode->getId();

    const auto reg = ctx.regMgr.alloc();
    if (reg == ScratchRegister::NONE)
    {
      if (ctx.isLocalVariable(exprId))
      {
        ctx.textSection << "push -" << ctx.getLocalVariableStackOffset(exprId)
                        << "(%rbp)\n";
      }
      else if (ctx.isArgument(exprId))
      {
        ctx.textSection << "push " << ctx.getArgumentStackOffset(exprId)
                        << "(%rbp)\n";
      }
      else
      {
        ctx.textSection << "push " << exprId << "\n";
      }
    }
    else
    {
      if (ctx.isLocalVariable(exprId))
      {
        ctx.textSection << "mov -" << ctx.getLocalVariableStackOffset(exprId)
                        << "(%rbp), " << scratchRegisterStringMap.at(reg)
                        << "\n";
      }
      else if (ctx.isArgument(exprId))
      {
        ctx.textSection << "mov " << ctx.getArgumentStackOffset(exprId)
                        << "(%rbp), " << scratchRegisterStringMap.at(reg)
                        << "\n";
      }
      else
      {
        ctx.textSection << "mov " << exprId << ", "
                        << scratchRegisterStringMap.at(reg) << "\n";
      }
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
    const auto assignId = assignStmtNode->getLValueId();

    const auto reg = cgen(assignStmtNode->getExprNode(), ctx);

    if (reg == ScratchRegister::NONE)
    {
      if (ctx.isLocalVariable(assignId))
      {
        ctx.textSection << "pop -" << ctx.getLocalVariableStackOffset(assignId)
                        << "(%rbp)\n";
      }
      else if (ctx.isArgument(assignId))
      {
        ctx.textSection << "pop " << ctx.getArgumentStackOffset(assignId)
                        << "(%rbp)\n";
      }
      else
      {
        ctx.textSection << "pop " << assignId << "\n";
      }
    }
    else
    {
      if (ctx.isLocalVariable(assignId))
      {
        ctx.textSection << "mov " << scratchRegisterStringMap.at(reg) << ", -"
                        << ctx.getLocalVariableStackOffset(assignId)
                        << "(%rbp)\n";
      }
      else if (ctx.isArgument(assignId))
      {
        ctx.textSection << "mov " << scratchRegisterStringMap.at(reg) << ", "
                        << ctx.getArgumentStackOffset(assignId) << "(%rbp)\n";
      }
      else
      {
        ctx.textSection << "mov " << scratchRegisterStringMap.at(reg) << ", "
                        << assignId << "\n";
      }

      ctx.regMgr.free(reg);
    }

    break;
  }
  case parser::StmtNodeType::CALL_STMT: {
    const auto callStmtNode =
        std::dynamic_pointer_cast<parser::CallStmtNode>(stmtNode);

    // push arguments onto stack
    for (const auto &argument : callStmtNode->getArguments())
    {
      const auto reg = cgen(argument, ctx);

      if (reg != ScratchRegister::NONE)
        ctx.textSection << "push " << scratchRegisterStringMap.at(reg) << "\n";
      // else already pushed onto stack

      ctx.regMgr.free(reg);
    }

    ctx.textSection << "call " << callStmtNode->getProcId() << "\n";

    // remove call arguments from frame
    const auto frameOffset = callStmtNode->getArguments().size() * 8;
    if (frameOffset > 0)
      ctx.textSection << "add $" << frameOffset << ", %rsp\n";

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

// forward declaration for declNode cgen function
auto cgen(const parser::BlockNode &blockNode, CgenContext &ctx) -> void;

auto cgen(std::shared_ptr<parser::DeclNode> declNode, CgenContext &ctx) -> void
{
  switch (declNode->getType())
  {
  case parser::DeclNodeType::CONST_DECL: {
    const auto constDeclNode =
        std::dynamic_pointer_cast<parser::ConstDeclNode>(declNode);
    const auto exprToken = constDeclNode->getDeclExprToken();

    switch (exprToken->getType())
    {
    case lexer::TokenType::ID: {
      const auto exprId =
          std::dynamic_pointer_cast<lexer::IdToken>(exprToken)->getId();

      if (ctx.isLocalVariable(exprId))
      {
        ctx.textSection << "push -" << ctx.getLocalVariableStackOffset(exprId)
                        << "(%rbp)\n";
        ctx.localVariables.push_back(constDeclNode->getDeclId());
      }
      else if (ctx.isArgument(exprId))
      {
        ctx.textSection << "push " << ctx.getArgumentStackOffset(exprId)
                        << "(%rbp)\n";
        ctx.localVariables.push_back(constDeclNode->getDeclId());
      }
      else
      {
        ctx.roDataSection << ".set " << constDeclNode->getDeclId() << ", "
                          << exprId << "\n";
      }

      break;
    }
    case lexer::TokenType::INT_LITERAL: {
      const auto exprInt =
          std::dynamic_pointer_cast<lexer::IntegerToken>(exprToken)->getInt();

      if (ctx.isProcedureScope)
      {
        ctx.textSection << "push $" << exprInt << "\n";
        ctx.localVariables.push_back(constDeclNode->getDeclId());
      }
      else
      {
        ctx.roDataSection << constDeclNode->getDeclId() << ": .value "
                          << exprInt << "\n";
      }

      break;
    }
    default: {
      const auto errMsg = std::string{
          "codegen error: unsupported expression type for const declaration."};
      assert(errMsg.c_str() && false);
    }
    }

    break;
  }
  case parser::DeclNodeType::PROC_DECL: {
    const auto procDeclNode =
        std::dynamic_pointer_cast<parser::ProcDeclNode>(declNode);

    ctx.globalSection << ", " << procDeclNode->getDeclId();

    const auto oldIsProcedureScope = ctx.isProcedureScope;
    const auto oldArguments = ctx.arguments;
    const auto oldLocalVariables = ctx.localVariables;
    auto oldTextSection = std::stringstream{ctx.textSection.str()};

    ctx.textSection.str(std::string{});
    ctx.textSection << procDeclNode->getDeclId() << ":\n"
                    << "push %rbp\n"
                    << "mov %rsp, %rbp\n";

    ctx.isProcedureScope = true;
    ctx.arguments = procDeclNode->getArguments();
    cgen(*std::dynamic_pointer_cast<parser::BlockNode>(
             procDeclNode->getDeclBlockNode()),
         ctx);

    ctx.textSection << "mov %rbp, %rsp\n"
                    << "pop %rbp\n"
                    << "ret\n";

    ctx.isProcedureScope = oldIsProcedureScope;
    ctx.arguments = oldArguments;
    ctx.localVariables = oldLocalVariables;
    ctx.textSection << oldTextSection.str();

    break;
  }
  case parser::DeclNodeType::VAR_DECL: {
    const auto varDeclNode =
        std::dynamic_pointer_cast<parser::VarDeclNode>(declNode);
    const auto varId = varDeclNode->getDeclId();

    if (ctx.isProcedureScope)
    {
      ctx.textSection << "push $0\n";
      ctx.localVariables.push_back(varId);
    }
    else
    {
      ctx.dataSection << varId << ": .value 0\n";
    }

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

  if (!ctx.isProcedureScope)
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

  ctx.roDataSection << "format: .asciz \"%d\\n\"\n";

  cgen(programNode.getBlockNode(), ctx);

  ctx.textSection << "push $1\n"
                  << "call exit" << std::endl;

  asmFile << ctx.roDataSection.rdbuf() << ctx.dataSection.rdbuf()
          << ctx.globalSection.rdbuf() << "\n.section .text\n"
          << ctx.textSection.rdbuf();
  asmFile.close();
}

}; // namespace codegen
}; // namespace pl0c
