#include "parser/parser.hh"

#include "lexer/token/id_token.hh"
#include "lexer/token/token_type.hh"
#include "parser/ast/block_node.hh"
#include "parser/ast/const_decl_node.hh"
#include "parser/ast/decl_node.hh"
#include "parser/ast/program_node.hh"

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
    default: {
      break;
    }
    }
  };

  parseDecl();
  return declarations;
}

[[nodiscard]] auto block(std::deque<std::shared_ptr<lexer::Token>> &tokens)
    -> BlockNode
{
  const auto declarations = declList(tokens);

  expect(lexer::TokenType::BEGIN, tokens);

  expect(lexer::TokenType::END, tokens);

  return BlockNode{declarations};
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
