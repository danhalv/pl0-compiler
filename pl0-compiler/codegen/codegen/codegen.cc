#include "codegen/codegen.hh"

#include "codegen/label_manager.hh"
#include "codegen/scratch_register_manager.hh"
#include "lexer/token/token_type.hh"
#include "parser/ast/assign_stmt_node.hh"
#include "parser/ast/block_node.hh"
#include "parser/ast/call_stmt_node.hh"
#include "parser/ast/comparison_test_node.hh"
#include "parser/ast/const_decl_node.hh"
#include "parser/ast/decl_node.hh"
#include "parser/ast/division_expr_node.hh"
#include "parser/ast/expr_node.hh"
#include "parser/ast/id_expr_node.hh"
#include "parser/ast/if_stmt_node.hh"
#include "parser/ast/int_expr_node.hh"
#include "parser/ast/minus_expr_node.hh"
#include "parser/ast/multiplication_expr_node.hh"
#include "parser/ast/out_stmt_node.hh"
#include "parser/ast/plus_expr_node.hh"
#include "parser/ast/proc_decl_node.hh"
#include "parser/ast/program_node.hh"
#include "parser/ast/stmt_node.hh"
#include "parser/ast/test_node.hh"
#include "parser/ast/var_decl_node.hh"
#include "parser/ast/while_stmt_node.hh"

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
        textSection{}, labelMgr{}, regMgr{}
  {
    dataSection << ".section .data\n";
    roDataSection << ".section .rodata\n";
    globalSection << ".global _start, printf, scanf, exit";
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
  LabelManager labelMgr;
  ScratchRegisterManager regMgr;
};

auto cgen(const std::shared_ptr<parser::ExprNode> exprNode, CgenContext &ctx)
    -> ScratchRegister
{
  auto generateArithmeticCode = [&ctx](const std::string &arithmeticInstruction,
                                       const ScratchRegister leftExprReg,
                                       const ScratchRegister rightExprReg) {
    if (leftExprReg == ScratchRegister::NONE &&
        rightExprReg == ScratchRegister::NONE)
    {
      ctx.textSection << "pop %rax\n"
                      << arithmeticInstruction << " %rsp, %rax\n"
                      << "pop\n"
                      << "push %rax\n";
      return ScratchRegister::NONE;
    }

    if (leftExprReg == ScratchRegister::NONE)
    {
      ctx.textSection << "pop %rax\n"
                      << arithmeticInstruction << " "
                      << scratchRegisterStringMap.at(rightExprReg)
                      << ", %rax\n";
      return rightExprReg;
    }

    if (rightExprReg == ScratchRegister::NONE)
    {
      ctx.textSection << "pop %rax\n"
                      << arithmeticInstruction << " "
                      << scratchRegisterStringMap.at(leftExprReg) << ", %rax\n";
      return leftExprReg;
    }

    ctx.textSection << arithmeticInstruction << " "
                    << scratchRegisterStringMap.at(leftExprReg) << ", "
                    << scratchRegisterStringMap.at(rightExprReg) << "\n";
    ctx.regMgr.free(leftExprReg);
    return rightExprReg;
  };

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
  case parser::ExprNodeType::INPUT_EXPR: {

    ctx.textSection << "sub $8, %rsp\n"
                    << "lea (%rsp), %rsi\n"
                    << "lea input_format, %rdi\n"
                    << "call scanf\n";

    return ScratchRegister::NONE;
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
  case parser::ExprNodeType::DIVISION_EXPR: {
    const auto divisionExprNode =
        std::dynamic_pointer_cast<parser::DivisionExprNode>(exprNode);

    // swapped orders of left and right expressions for idiv instruction
    const auto rightExprReg = cgen(divisionExprNode->getLhsExpr(), ctx);
    const auto leftExprReg = cgen(divisionExprNode->getRhsExpr(), ctx);

    // idiv works on rdx:rax
    ctx.textSection << "mov $0, %rdx\n";

    if (leftExprReg == ScratchRegister::NONE &&
        rightExprReg == ScratchRegister::NONE)
    {
      ctx.textSection << "pop %rax\n"
                      << "idiv %rsp\n"
                      << "pop\n"
                      << "push %rax\n";
      return ScratchRegister::NONE;
    }

    if (leftExprReg == ScratchRegister::NONE)
    {
      ctx.textSection << "pop %rax\n"
                      << "idiv " << scratchRegisterStringMap.at(rightExprReg)
                      << "\n"
                      << "mov %rax, "
                      << scratchRegisterStringMap.at(rightExprReg) << "\n";
      return rightExprReg;
    }

    if (rightExprReg == ScratchRegister::NONE)
    {
      ctx.textSection << "pop %rax\n"
                      << "idiv " << scratchRegisterStringMap.at(leftExprReg)
                      << "\n"
                      << "mov %rax, "
                      << scratchRegisterStringMap.at(leftExprReg) << "\n";
      return leftExprReg;
    }

    ctx.textSection << "mov " << scratchRegisterStringMap.at(rightExprReg)
                    << ", %rax\n"
                    << "idiv " << scratchRegisterStringMap.at(leftExprReg)
                    << "\n"
                    << "mov %rax, " << scratchRegisterStringMap.at(rightExprReg)
                    << "\n";
    ctx.regMgr.free(leftExprReg);
    return rightExprReg;
  }
  case parser::ExprNodeType::MINUS_EXPR: {
    const auto minusExprNode =
        std::dynamic_pointer_cast<parser::MinusExprNode>(exprNode);

    const auto leftExprReg = cgen(minusExprNode->getLhsExpr(), ctx);
    const auto rightExprReg = cgen(minusExprNode->getRhsExpr(), ctx);

    // swapped order of left and right expressions for sub instruction
    return generateArithmeticCode("sub", rightExprReg, leftExprReg);
  }
  case parser::ExprNodeType::MULTIPLICATION_EXPR: {
    const auto multiplicationExprNode =
        std::dynamic_pointer_cast<parser::MultiplicationExprNode>(exprNode);

    const auto leftExprReg = cgen(multiplicationExprNode->getLhsExpr(), ctx);
    const auto rightExprReg = cgen(multiplicationExprNode->getRhsExpr(), ctx);

    return generateArithmeticCode("imul", leftExprReg, rightExprReg);
  }
  case parser::ExprNodeType::PLUS_EXPR: {
    const auto plusExprNode =
        std::dynamic_pointer_cast<parser::PlusExprNode>(exprNode);

    const auto leftExprReg = cgen(plusExprNode->getLhsExpr(), ctx);
    const auto rightExprReg = cgen(plusExprNode->getRhsExpr(), ctx);

    return generateArithmeticCode("add", leftExprReg, rightExprReg);
  }
  default: {
    const auto errMsg =
        std::string{"codegen error: unsupported expression type."};
    assert(errMsg.c_str() && false);
  }
  }

  return ScratchRegister::NONE;
}

auto cgen(const std::shared_ptr<parser::TestNode> testNode, CgenContext &ctx)
    -> ScratchRegister
{
  auto generateComparisonCodeWithConditionalJump =
      [&testNode, &ctx](const std::string &jumpInstruction) {
        const auto comparisonTestNode =
            std::dynamic_pointer_cast<parser::ComparisonTestNode>(testNode);

        const auto leftExprReg =
            cgen(comparisonTestNode->getLhsExprNode(), ctx);
        const auto rightExprReg =
            cgen(comparisonTestNode->getRhsExprNode(), ctx);

        const auto cmpTrueLabel = ctx.labelMgr.genLabel("le_true");
        const auto cmpFalseLabel = ctx.labelMgr.genLabel("le_false");
        const auto cmpDoneLabel = ctx.labelMgr.genLabel("le_done");

        if (leftExprReg == ScratchRegister::NONE &&
            rightExprReg == ScratchRegister::NONE)
        {
          ctx.textSection << "cmp %rsp, 8(%rsp)\n"
                          << "add $16, %rsp\n"
                          << jumpInstruction << " " << cmpTrueLabel << "\n"
                          << cmpFalseLabel << ":\n"
                          << "push $0\n"
                          << "jmp " << cmpDoneLabel << "\n"
                          << cmpTrueLabel << ":\n"
                          << "push $1\n"
                          << cmpDoneLabel << ":\n";
          return ScratchRegister::NONE;
        }

        if (leftExprReg == ScratchRegister::NONE)
        {
          ctx.textSection << "cmp " << scratchRegisterStringMap.at(leftExprReg)
                          << ", %rsp\n"
                          << "pop\n"
                          << jumpInstruction << " " << cmpTrueLabel << "\n"
                          << cmpFalseLabel << ":\n"
                          << "mov $0, "
                          << scratchRegisterStringMap.at(rightExprReg) << "\n"
                          << "jmp " << cmpDoneLabel << "\n"
                          << cmpTrueLabel << ":\n"
                          << "mov $1, "
                          << scratchRegisterStringMap.at(rightExprReg) << "\n"
                          << cmpDoneLabel << ":\n";
          return rightExprReg;
        }

        if (rightExprReg == ScratchRegister::NONE)
        {
          ctx.textSection << "cmp %rsp, "
                          << scratchRegisterStringMap.at(leftExprReg) << "\n"
                          << "pop\n"
                          << jumpInstruction << " " << cmpTrueLabel << "\n"
                          << cmpFalseLabel << ":\n"
                          << "mov $0, "
                          << scratchRegisterStringMap.at(leftExprReg) << "\n"
                          << "jmp " << cmpDoneLabel << "\n"
                          << cmpTrueLabel << ":\n"
                          << "mov $1, "
                          << scratchRegisterStringMap.at(leftExprReg) << "\n"
                          << cmpDoneLabel << ":\n";
          return leftExprReg;
        }

        ctx.textSection << "cmp " << scratchRegisterStringMap.at(rightExprReg)
                        << ", " << scratchRegisterStringMap.at(leftExprReg)
                        << "\n"
                        << jumpInstruction << " " << cmpTrueLabel << "\n"
                        << cmpFalseLabel << ":\n"
                        << "mov $0, "
                        << scratchRegisterStringMap.at(rightExprReg) << "\n"
                        << "jmp " << cmpDoneLabel << "\n"
                        << cmpTrueLabel << ":\n"
                        << "mov $1, "
                        << scratchRegisterStringMap.at(rightExprReg) << "\n"
                        << cmpDoneLabel << ":\n";
        ctx.regMgr.free(leftExprReg);
        return rightExprReg;
      };

  switch (testNode->getType())
  {
  case parser::TestNodeType::EQ_TEST: {
    return generateComparisonCodeWithConditionalJump("je");
  }
  case parser::TestNodeType::LE_TEST: {
    return generateComparisonCodeWithConditionalJump("jl");
  }
  case parser::TestNodeType::LEQ_TEST: {
    return generateComparisonCodeWithConditionalJump("jle");
  }
  case parser::TestNodeType::GE_TEST: {
    return generateComparisonCodeWithConditionalJump("jg");
  }
  case parser::TestNodeType::GEQ_TEST: {
    return generateComparisonCodeWithConditionalJump("jge");
  }
  case parser::TestNodeType::NEQ_TEST: {
    return generateComparisonCodeWithConditionalJump("jne");
  }
  default: {
    const auto errMsg = std::string{"codegen error: unsupported test type."};
    assert(errMsg.c_str() && false);
  }
  }
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
  case parser::StmtNodeType::IF_STMT: {
    const auto ifStmtNode =
        std::dynamic_pointer_cast<parser::IfStmtNode>(stmtNode);

    const auto testReg = cgen(ifStmtNode->getTestNode(), ctx);

    const auto ifTrueLabel = ctx.labelMgr.genLabel("if_true");
    const auto ifDoneLabel = ctx.labelMgr.genLabel("if_done");

    if (testReg == ScratchRegister::NONE)
    {
      ctx.textSection << "cmp $1, %rsp\n"
                      << "pop\n";
    }
    else
    {
      ctx.textSection << "cmp $1, " << scratchRegisterStringMap.at(testReg)
                      << "\n";

      ctx.regMgr.free(testReg);
    }

    ctx.textSection << "jne " << ifDoneLabel << "\n" << ifTrueLabel << ":\n";

    for (const auto &statement : ifStmtNode->getStatements())
      cgen(statement, ctx);

    ctx.textSection << ifDoneLabel << ":\n";

    break;
  }
  case parser::StmtNodeType::OUT_STMT: {
    const auto exprNode =
        std::dynamic_pointer_cast<parser::OutStmtNode>(stmtNode)->getExprNode();

    auto reg = cgen(exprNode, ctx);

    if (reg == ScratchRegister::NONE)
    {
      ctx.textSection << "pop %rsi\n"
                      << "lea output_format, %rdi\n"
                      << "call printf\n";
    }
    else
    {
      ctx.textSection << "mov " << scratchRegisterStringMap.at(reg)
                      << ", %rsi\n"
                      << "lea output_format, %rdi\n"
                      << "call printf\n";
      ctx.regMgr.free(reg);
    }

    break;
  }
  case parser::StmtNodeType::WHILE_STMT: {
    const auto whileStmtNode =
        std::dynamic_pointer_cast<parser::WhileStmtNode>(stmtNode);

    const auto loopLabel = ctx.labelMgr.genLabel("loop");
    const auto loopDoneLabel = ctx.labelMgr.genLabel("loop");

    ctx.textSection << loopLabel << ":\n";

    const auto testReg = cgen(whileStmtNode->getTestNode(), ctx);

    if (testReg == ScratchRegister::NONE)
    {
      ctx.textSection << "cmp $1, %rsp\n"
                      << "pop\n";
    }
    else
    {
      ctx.textSection << "cmp $1, " << scratchRegisterStringMap.at(testReg)
                      << "\n";
    }

    ctx.textSection << "jne " << loopDoneLabel << "\n";

    for (const auto &statement : whileStmtNode->getStatements())
      cgen(statement, ctx);

    ctx.textSection << "jmp " << loopLabel << "\n" << loopDoneLabel << ":\n";

    ctx.regMgr.free(testReg);
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
        ctx.roDataSection << constDeclNode->getDeclId() << ": .quad " << exprInt
                          << "\n";
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
      ctx.dataSection << varId << ": .quad 0\n";
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

  ctx.roDataSection << "input_format: .asciz \"%d\"\n";
  ctx.roDataSection << "output_format: .asciz \"%d\\n\"\n";

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
