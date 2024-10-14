#include "parser/parser.hh"

#include "lexer/token/id_token.hh"
#include "lexer/token/token_type.hh"
#include "parser/ast/assign_stmt_node.hh"
#include "parser/ast/block_node.hh"
#include "parser/ast/const_decl_node.hh"
#include "parser/ast/decl_node.hh"
#include "parser/ast/expr_node.hh"
#include "parser/ast/id_expr_node.hh"
#include "parser/ast/input_expr_node.hh"
#include "parser/ast/int_expr_node.hh"
#include "parser/ast/negative_expr_node.hh"
#include "parser/ast/plus_expr_node.hh"
#include "parser/ast/proc_decl_node.hh"
#include "parser/ast/program_node.hh"
#include "parser/ast/stmt_node.hh"
#include "parser/ast/var_decl_node.hh"

#include <cassert>
#include <deque>
#include <functional>
#include <memory>
#include <utility>
#include <vector>

namespace pl0c
{
namespace parser
{

auto next(std::deque<std::shared_ptr<lexer::Token>> &tokens) -> void
{
  if (tokens.size() > 0)
  {
    tokens.pop_front();

    while (tokens.size() > 0 &&
           lexer::TokenType::WHITESPACE == tokens.front()->getType())
    {
      tokens.pop_front();
    }
  }
}

auto expect(const lexer::TokenType expectedTokenType,
            std::deque<std::shared_ptr<lexer::Token>> &tokens)
    -> std::shared_ptr<lexer::Token>
{
  auto currentToken = tokens.front();

  const auto errMsg = std::string{
      "parser error: expected " +
      lexer::tokenTypeNameMap.at(expectedTokenType) + " token, but found " +
      lexer::tokenTypeNameMap.at(currentToken->getType()) + "token."};
  assert((expectedTokenType == currentToken->getType()) && errMsg.c_str());

  next(tokens);

  return currentToken;
}

[[nodiscard]] auto constExpr(std::deque<std::shared_ptr<lexer::Token>> &tokens)
    -> std::shared_ptr<lexer::Token>
{
  if (lexer::TokenType::INT_LITERAL == tokens.front()->getType())
  {
    return expect(lexer::TokenType::INT_LITERAL, tokens);
  }

  return expect(lexer::TokenType::ID, tokens);
}

[[nodiscard]] auto type(std::deque<std::shared_ptr<lexer::Token>> &tokens)
    -> std::shared_ptr<lexer::Token>
{
  return expect(lexer::TokenType::INT, tokens);
}

[[nodiscard]]
auto constDeclItem(std::deque<std::shared_ptr<lexer::Token>> &tokens)
    -> std::shared_ptr<ConstDeclNode>
{
  const auto declIdToken = expect(lexer::TokenType::ID, tokens);
  const auto declId =
      std::dynamic_pointer_cast<lexer::IdToken>(declIdToken)->getId();

  expect(lexer::TokenType::COLON, tokens);

  const auto declTypeToken = type(tokens);

  expect(lexer::TokenType::ASSIGNMENT, tokens);

  const auto declConstExprToken = constExpr(tokens);

  return std::make_shared<ConstDeclNode>(declId, declTypeToken,
                                         declConstExprToken);
}

[[nodiscard]] auto constDecl(std::deque<std::shared_ptr<lexer::Token>> &tokens)
    -> std::vector<std::shared_ptr<ConstDeclNode>>
{
  auto constDeclarations = std::vector<std::shared_ptr<ConstDeclNode>>{};

  expect(lexer::TokenType::CONST, tokens);

  constDeclarations.push_back(constDeclItem(tokens));

  while (lexer::TokenType::COMMA == tokens.front()->getType())
  {
    next(tokens);
    constDeclarations.push_back(constDeclItem(tokens));
  }

  expect(lexer::TokenType::SEMI_COLON, tokens);

  return constDeclarations;
}

[[nodiscard]] auto formalDecl(std::deque<std::shared_ptr<lexer::Token>> &tokens)
    -> std::pair<std::string, std::shared_ptr<lexer::Token>>
{
  const auto argumentIdToken = expect(lexer::TokenType::ID, tokens);
  const auto argumentId =
      std::dynamic_pointer_cast<lexer::IdToken>(argumentIdToken)->getId();

  expect(lexer::TokenType::COLON, tokens);

  const auto argumentTypeToken = type(tokens);

  return std::make_pair(argumentId, argumentTypeToken);
}

// forward declaration for procDecl function
[[nodiscard]] auto block(std::deque<std::shared_ptr<lexer::Token>> &tokens)
    -> BlockNode;

[[nodiscard]] auto procDecl(std::deque<std::shared_ptr<lexer::Token>> &tokens)
    -> std::shared_ptr<ProcDeclNode>
{
  expect(lexer::TokenType::PROCEDURE, tokens);

  const auto declIdToken = expect(lexer::TokenType::ID, tokens);
  const auto declId =
      std::dynamic_pointer_cast<lexer::IdToken>(declIdToken)->getId();

  expect(lexer::TokenType::LEFT_PAREN, tokens);

  auto procArguments =
      std::vector<std::pair<std::string, std::shared_ptr<lexer::Token>>>{};
  if (lexer::TokenType::ID == tokens.front()->getType())
  {
    procArguments.push_back(formalDecl(tokens));

    while (lexer::TokenType::COMMA == tokens.front()->getType())
    {
      next(tokens);
      procArguments.push_back(formalDecl(tokens));
    }
  }

  expect(lexer::TokenType::RIGHT_PAREN, tokens);

  expect(lexer::TokenType::SEMI_COLON, tokens);

  const auto procBlockNode = block(tokens);

  const auto endProcIdToken = expect(lexer::TokenType::ID, tokens);
  const auto endProcId =
      std::dynamic_pointer_cast<lexer::IdToken>(endProcIdToken)->getId();

  const auto errMsg =
      std::string{"parser error: procedure must have same identifier at "
                  "beginning and end. Procedure identifier at start: " +
                  declId + ", and at end: " + endProcId};
  assert((declId == endProcId) && errMsg.c_str());

  expect(lexer::TokenType::SEMI_COLON, tokens);

  return std::make_shared<ProcDeclNode>(
      declId, procArguments, std::make_shared<BlockNode>(procBlockNode));
}

[[nodiscard]]
auto varDeclItem(std::deque<std::shared_ptr<lexer::Token>> &tokens)
    -> std::shared_ptr<VarDeclNode>
{
  const auto declIdToken = expect(lexer::TokenType::ID, tokens);
  const auto declId =
      std::dynamic_pointer_cast<lexer::IdToken>(declIdToken)->getId();

  expect(lexer::TokenType::COLON, tokens);

  const auto declTypeToken = type(tokens);

  return std::make_shared<VarDeclNode>(declId, declTypeToken);
}

[[nodiscard]] auto varDecl(std::deque<std::shared_ptr<lexer::Token>> &tokens)
    -> std::vector<std::shared_ptr<VarDeclNode>>
{
  auto varDeclarations = std::vector<std::shared_ptr<VarDeclNode>>{};

  expect(lexer::TokenType::VAR, tokens);

  varDeclarations.push_back(varDeclItem(tokens));

  while (lexer::TokenType::COMMA == tokens.front()->getType())
  {
    next(tokens);
    varDeclarations.push_back(varDeclItem(tokens));
  }

  expect(lexer::TokenType::SEMI_COLON, tokens);

  return varDeclarations;
}

[[nodiscard]] auto declList(std::deque<std::shared_ptr<lexer::Token>> &tokens)
    -> std::vector<std::shared_ptr<DeclNode>>
{
  auto declarations = std::vector<std::shared_ptr<DeclNode>>{};

  std::function<void()> parseDecl; // declaration enables recursive call
  parseDecl = [&tokens, &declarations, &parseDecl]() -> void {
    switch (tokens.front()->getType())
    {
    case lexer::TokenType::CONST: {
      const auto constDeclarations = constDecl(tokens);
      declarations.insert(declarations.end(), constDeclarations.begin(),
                          constDeclarations.end());
      parseDecl();
      break;
    }
    case lexer::TokenType::VAR: {
      const auto varDeclarations = varDecl(tokens);
      declarations.insert(declarations.end(), varDeclarations.begin(),
                          varDeclarations.end());
      parseDecl();
      break;
    }
    case lexer::TokenType::PROCEDURE: {
      const auto procDeclaration = procDecl(tokens);
      declarations.push_back(procDeclaration);
      parseDecl();
      break;
    }
    default: {
      break;
    }
    }
  };

  parseDecl();
  return declarations;
}

// forward declaration for factor function
[[nodiscard]] auto expr(std::deque<std::shared_ptr<lexer::Token>> &tokens)
    -> std::shared_ptr<ExprNode>;

[[nodiscard]] auto factor(std::deque<std::shared_ptr<lexer::Token>> &tokens)
    -> std::shared_ptr<ExprNode>
{
  switch (tokens.front()->getType())
  {
  case lexer::TokenType::ID: {
    const auto factorIdToken = expect(lexer::TokenType::ID, tokens);
    const auto factorId =
        std::dynamic_pointer_cast<lexer::IdToken>(factorIdToken)->getId();

    return std::make_shared<IdExprNode>(factorId);
  }
  case lexer::TokenType::INPUT: {
    next(tokens);
    return std::make_shared<InputExprNode>();
  }
  case lexer::TokenType::INT_LITERAL: {
    const auto integerToken = expect(lexer::TokenType::INT_LITERAL, tokens);
    const auto integerValue =
        std::dynamic_pointer_cast<lexer::IntegerToken>(integerToken)->getInt();

    return std::make_shared<IntExprNode>(integerValue);
  }
  case lexer::TokenType::LEFT_PAREN: {
    next(tokens);
    const auto exprNode = expr(tokens);

    expect(lexer::TokenType::RIGHT_PAREN, tokens);

    return exprNode;
  }
  case lexer::TokenType::MINUS: {
    next(tokens);
    return std::make_shared<NegativeExprNode>(factor(tokens));
  }
  default: {
    const auto errMsg = std::string{
        "parser error: unknown start of expression with " +
        lexer::tokenTypeNameMap.at(tokens.front()->getType()) + " token."};
    assert(errMsg.c_str() && false);
  }
  }

  return nullptr;
}

[[nodiscard]] auto term(std::deque<std::shared_ptr<lexer::Token>> &tokens)
    -> std::shared_ptr<ExprNode>
{
  const auto lhsExprNode = factor(tokens);

  return lhsExprNode;
}

[[nodiscard]] auto expr(std::deque<std::shared_ptr<lexer::Token>> &tokens)
    -> std::shared_ptr<ExprNode>
{
  const auto lhsExprNode = term(tokens);

  if (lexer::TokenType::PLUS == tokens.front()->getType())
  {
    next(tokens);
    return std::make_shared<PlusExprNode>(lhsExprNode, term(tokens));
  }

  return lhsExprNode;
}

[[nodiscard]] auto assignStmt(std::deque<std::shared_ptr<lexer::Token>> &tokens)
    -> std::shared_ptr<AssignStmtNode>
{
  const auto lvalueIdToken = expect(lexer::TokenType::ID, tokens);
  const auto lvalueId =
      std::dynamic_pointer_cast<lexer::IdToken>(lvalueIdToken)->getId();

  expect(lexer::TokenType::WALRUS, tokens);

  const auto exprNode = expr(tokens);

  expect(lexer::TokenType::SEMI_COLON, tokens);

  return std::make_shared<AssignStmtNode>(lvalueId, exprNode);
}

[[nodiscard]] auto stmtList(std::deque<std::shared_ptr<lexer::Token>> &tokens)
    -> std::vector<std::shared_ptr<StmtNode>>
{
  auto statements = std::vector<std::shared_ptr<StmtNode>>{};

  std::function<void()> parseStmt; // declaration enables recursive call
  parseStmt = [&tokens, &statements, &parseStmt]() -> void {
    switch (tokens.front()->getType())
    {
    case lexer::TokenType::ID: {
      statements.push_back(assignStmt(tokens));
      parseStmt();
      break;
    }
    default: {
      break;
    }
    }
  };

  parseStmt();
  return statements;
}

[[nodiscard]] auto block(std::deque<std::shared_ptr<lexer::Token>> &tokens)
    -> BlockNode
{
  const auto declarations = declList(tokens);

  expect(lexer::TokenType::BEGIN, tokens);

  const auto statements = stmtList(tokens);

  expect(lexer::TokenType::END, tokens);

  return BlockNode{declarations, statements};
}

[[nodiscard]] auto program(std::deque<std::shared_ptr<lexer::Token>> &tokens)
    -> ProgramNode
{
  expect(lexer::TokenType::MODULE, tokens);

  const auto beginProgramIdToken = expect(lexer::TokenType::ID, tokens);

  expect(lexer::TokenType::SEMI_COLON, tokens);

  const auto programBlockNode = block(tokens);

  const auto endProgramIdToken = expect(lexer::TokenType::ID, tokens);

  const auto beginProgramId =
      std::dynamic_pointer_cast<lexer::IdToken>(beginProgramIdToken)->getId();
  const auto endProgramId =
      std::dynamic_pointer_cast<lexer::IdToken>(endProgramIdToken)->getId();

  const auto errMsg =
      std::string{"parser error: module must have same identifier at "
                  "beginning and end. Module identifier at start: " +
                  beginProgramId + ", and at end: " + endProgramId};
  assert((beginProgramId == endProgramId) && errMsg.c_str());

  expect(lexer::TokenType::DOT, tokens);

  return ProgramNode{beginProgramId, programBlockNode};
}

auto run(const std::vector<std::shared_ptr<lexer::Token>> &tokens)
    -> ProgramNode
{
  auto tokenDeque =
      std::deque<std::shared_ptr<lexer::Token>>{tokens.begin(), tokens.end()};

  return program(tokenDeque);
}

}; // namespace parser
}; // namespace pl0c
