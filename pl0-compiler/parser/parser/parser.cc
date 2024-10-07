#include "parser/parser.hh"

#include "lexer/token/id_token.hh"
#include "lexer/token/token_type.hh"
#include "parser/ast/block_node.hh"
#include "parser/ast/program_node.hh"

#include <cassert>
#include <deque>
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

[[nodiscard]] auto block(std::deque<std::shared_ptr<lexer::Token>> &tokens)
    -> BlockNode
{
  expect(lexer::TokenType::BEGIN, tokens);
  expect(lexer::TokenType::END, tokens);

  return BlockNode{};
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
