#include "analyzer/analyzer.hh"

#include "parser/ast/assign_stmt_node.hh"
#include "parser/ast/block_node.hh"
#include "parser/ast/call_stmt_node.hh"
#include "parser/ast/comparison_test_node.hh"
#include "parser/ast/const_decl_node.hh"
#include "parser/ast/decl_node.hh"
#include "parser/ast/division_expr_node.hh"
#include "parser/ast/id_expr_node.hh"
#include "parser/ast/if_stmt_node.hh"
#include "parser/ast/minus_expr_node.hh"
#include "parser/ast/multiplication_expr_node.hh"
#include "parser/ast/negative_expr_node.hh"
#include "parser/ast/odd_test_node.hh"
#include "parser/ast/out_stmt_node.hh"
#include "parser/ast/plus_expr_node.hh"
#include "parser/ast/proc_decl_node.hh"
#include "parser/ast/stmt_node.hh"
#include "parser/ast/test_node.hh"
#include "parser/ast/var_decl_node.hh"
#include "parser/ast/while_stmt_node.hh"

#include <cassert>
#include <memory>
#include <set>
#include <string>

namespace pl0c
{
namespace analyzer
{

struct AnalyzerContext
{
  AnalyzerContext()
      : isProcedureScope{false}, globalConstants{}, globalVariables{},
        procedureDeclarations{}, localConstants{}, localVariables{}
  {
  }

  auto idExistsLocally(const std::string &id) -> bool
  {
    return ((localConstants.find(id) != localConstants.end()) ||
            (localVariables.find(id) != localVariables.end()) ||
            (procedureDeclarations.find(id) != procedureDeclarations.end()));
  }

  auto idExistsGlobally(const std::string &id) -> bool
  {
    return ((globalConstants.find(id) != globalConstants.end()) ||
            (globalVariables.find(id) != globalVariables.end()) ||
            (procedureDeclarations.find(id) != procedureDeclarations.end()));
  }

  auto idExists(const std::string &id) -> bool
  {
    return ((globalConstants.find(id) != globalConstants.end()) ||
            (globalVariables.find(id) != globalVariables.end()) ||
            (procedureDeclarations.find(id) != procedureDeclarations.end()) ||
            (localConstants.find(id) != localConstants.end()) ||
            (localVariables.find(id) != localVariables.end()));
  }

  bool isProcedureScope;
  std::set<std::string> globalConstants;
  std::set<std::string> globalVariables;
  std::set<std::string> procedureDeclarations;
  std::set<std::string> localConstants;
  std::set<std::string> localVariables;
};

auto analyze(const std::shared_ptr<parser::ExprNode> exprNode,
             AnalyzerContext &ctx) -> void
{
  switch (exprNode->getType())
  {
  case parser::ExprNodeType::ID_EXPR: {
    const auto idExprNode =
        std::dynamic_pointer_cast<parser::IdExprNode>(exprNode);
    const auto id = idExprNode->getId();

    if ((ctx.localConstants.find(id) == ctx.localConstants.end()) &&
        (ctx.localVariables.find(id) == ctx.localVariables.end()) &&
        (ctx.globalConstants.find(id) == ctx.globalConstants.end()) &&
        (ctx.globalVariables.find(id) == ctx.globalVariables.end()))
    {
      const auto errMsg = std::string{"analyzer error: expression id: " + id +
                                      ", has not been declared."};
      assert(errMsg.c_str() && false);
    }
  }
  case parser::ExprNodeType::DIVISION_EXPR: {
    const auto divExprNode =
        std::dynamic_pointer_cast<parser::DivisionExprNode>(exprNode);

    analyze(divExprNode->getLhsExpr(), ctx);
    analyze(divExprNode->getRhsExpr(), ctx);

    break;
  }
  case parser::ExprNodeType::MULTIPLICATION_EXPR: {
    const auto multExprNode =
        std::dynamic_pointer_cast<parser::MultiplicationExprNode>(exprNode);

    analyze(multExprNode->getLhsExpr(), ctx);
    analyze(multExprNode->getRhsExpr(), ctx);

    break;
  }
  case parser::ExprNodeType::PLUS_EXPR: {
    const auto plusExprNode =
        std::dynamic_pointer_cast<parser::PlusExprNode>(exprNode);

    analyze(plusExprNode->getLhsExpr(), ctx);
    analyze(plusExprNode->getRhsExpr(), ctx);

    break;
  }
  case parser::ExprNodeType::MINUS_EXPR: {
    const auto minusExprNode =
        std::dynamic_pointer_cast<parser::MinusExprNode>(exprNode);

    analyze(minusExprNode->getLhsExpr(), ctx);
    analyze(minusExprNode->getRhsExpr(), ctx);

    break;
  }
  case parser::ExprNodeType::NEGATIVE_EXPR: {
    const auto negativeExprNode =
        std::dynamic_pointer_cast<parser::NegativeExprNode>(exprNode);

    analyze(negativeExprNode->getExprNode(), ctx);

    break;
  }
  default: {
    break;
  }
  }
}

auto analyze(const std::shared_ptr<parser::TestNode> testNode,
             AnalyzerContext &ctx) -> void
{
  switch (testNode->getType())
  {
  case parser::TestNodeType::EQ_TEST:
  case parser::TestNodeType::GE_TEST:
  case parser::TestNodeType::GEQ_TEST:
  case parser::TestNodeType::LE_TEST:
  case parser::TestNodeType::LEQ_TEST:
  case parser::TestNodeType::NEQ_TEST: {
    const auto comparisonTestNode =
        std::dynamic_pointer_cast<parser::ComparisonTestNode>(testNode);

    analyze(comparisonTestNode->getLhsExprNode(), ctx);
    analyze(comparisonTestNode->getRhsExprNode(), ctx);

    break;
  }
  case parser::TestNodeType::ODD_TEST: {
    const auto oddTestNode =
        std::dynamic_pointer_cast<parser::OddTestNode>(testNode);

    analyze(oddTestNode->getExprNode(), ctx);

    break;
  }
  default: {
    break;
  }
  }
}

// forward declaration for analyze(DeclNode) function
auto analyze(const parser::BlockNode &blockNode, AnalyzerContext &ctx) -> void;

auto analyze(const std::shared_ptr<parser::DeclNode> declNode,
             AnalyzerContext &ctx) -> void
{
  switch (declNode->getType())
  {
  case parser::DeclNodeType::CONST_DECL: {
    const auto constDeclNode =
        std::dynamic_pointer_cast<parser::ConstDeclNode>(declNode);
    const auto id = constDeclNode->getDeclId();

    if (ctx.isProcedureScope)
    {
      if (ctx.idExistsLocally(id))
      {
        const auto errMsg =
            std::string{"analyzer error: local variable id: " + id +
                        ", is already declared."};
        assert(errMsg.c_str() && false);
      }

      ctx.localVariables.emplace(id);
    }
    else
    {
      if (ctx.idExistsGlobally(id))
      {
        const auto errMsg =
            std::string{"analyzer error: global variable id: " + id +
                        ", is already declared."};
        assert(errMsg.c_str() && false);
      }

      ctx.globalVariables.emplace(id);
    }

    break;
  }
  case parser::DeclNodeType::PROC_DECL: {
    const auto procDeclNode =
        std::dynamic_pointer_cast<parser::ProcDeclNode>(declNode);
    const auto id = procDeclNode->getDeclId();

    if (ctx.idExists(id))
    {
      const auto errMsg = std::string{"analyzer error: procedure id: " + id +
                                      ", is already declared."};
      assert(errMsg.c_str() && false);
    }

    ctx.procedureDeclarations.emplace(id);

    const auto oldIsProcedureScope = ctx.isProcedureScope;
    const auto oldLocalVariables = ctx.localVariables;
    const auto oldLocalConstants = ctx.localConstants;

    ctx.isProcedureScope = true;
    for (const auto &[argumentId, _] : procDeclNode->getArguments())
      ctx.localVariables.emplace(argumentId);
    analyze(*procDeclNode->getDeclBlockNode(), ctx);

    ctx.isProcedureScope = oldIsProcedureScope;
    ctx.localVariables = oldLocalVariables;
    ctx.localConstants = oldLocalConstants;

    break;
  }
  case parser::DeclNodeType::VAR_DECL: {
    const auto varDeclNode =
        std::dynamic_pointer_cast<parser::VarDeclNode>(declNode);
    const auto id = varDeclNode->getDeclId();

    if (ctx.isProcedureScope)
    {
      if (ctx.idExistsLocally(id))
      {
        const auto errMsg =
            std::string{"analyzer error: local variable id: " + id +
                        ", is already declared."};
        assert(errMsg.c_str() && false);
      }

      ctx.localVariables.emplace(id);
    }
    else
    {
      if (ctx.idExistsGlobally(id))
      {
        const auto errMsg =
            std::string{"analyzer error: global variable id: " + id +
                        ", is already declared."};
        assert(errMsg.c_str() && false);
      }

      ctx.globalVariables.emplace(id);
    }

    break;
  }
  }
}

auto analyze(const std::shared_ptr<parser::StmtNode> stmtNode,
             AnalyzerContext &ctx) -> void
{
  switch (stmtNode->getType())
  {
  case parser::StmtNodeType::ASSIGN_STMT: {
    const auto assignId =
        std::dynamic_pointer_cast<parser::AssignStmtNode>(stmtNode)
            ->getLValueId();

    if ((ctx.localVariables.find(assignId) == ctx.localVariables.end()) &&
        (ctx.globalVariables.find(assignId) == ctx.globalVariables.end()))
    {
      const auto errMsg = std::string{
          "analyzer error: trying to assign to variable with id: " + assignId +
          ", which hasn't been declared."};
      assert(errMsg.c_str() && false);
    }

    break;
  }
  case parser::StmtNodeType::CALL_STMT: {
    const auto procId =
        std::dynamic_pointer_cast<parser::CallStmtNode>(stmtNode)->getProcId();

    if (ctx.procedureDeclarations.find(procId) ==
        ctx.procedureDeclarations.end())
    {
      const auto errMsg = std::string{
          "analyzer error: trying to call procedure with id: " + procId +
          ", which hasn't been declared."};
      assert(errMsg.c_str() && false);
    }

    break;
  }
  case parser::StmtNodeType::IF_STMT: {
    const auto ifStmtNode =
        std::dynamic_pointer_cast<parser::IfStmtNode>(stmtNode);

    analyze(ifStmtNode->getTestNode(), ctx);

    for (const auto &statement : ifStmtNode->getStatements())
      analyze(statement, ctx);

    break;
  }
  case parser::StmtNodeType::WHILE_STMT: {
    const auto whileStmtNode =
        std::dynamic_pointer_cast<parser::WhileStmtNode>(stmtNode);

    analyze(whileStmtNode->getTestNode(), ctx);

    for (const auto &statement : whileStmtNode->getStatements())
      analyze(statement, ctx);

    break;
  }
  case parser::StmtNodeType::OUT_STMT: {
    const auto outStmtNode =
        std::dynamic_pointer_cast<parser::OutStmtNode>(stmtNode);

    analyze(outStmtNode->getExprNode(), ctx);

    break;
  }
  default: {
    break;
  }
  }
}

auto analyze(const parser::BlockNode &blockNode, AnalyzerContext &ctx) -> void
{
  for (const auto &declaration : blockNode.getDeclarations())
    analyze(declaration, ctx);

  for (const auto &statement : blockNode.getStatements())
    analyze(statement, ctx);
}

auto run(const parser::ProgramNode &programNode) -> void
{
  AnalyzerContext ctx;

  analyze(programNode.getBlockNode(), ctx);
}

}; // namespace analyzer
}; // namespace pl0c
