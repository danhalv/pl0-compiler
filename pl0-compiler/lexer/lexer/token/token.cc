#include "lexer/token/token.hh"

namespace pl0c
{
namespace lexer
{

Token::Token(const TokenType type_) : type{type_}
{
}

bool operator==(const Token &lhs, const Token &rhs)
{
  return (lhs.type == rhs.type);
}

bool operator!=(const Token &lhs, const Token &rhs)
{
  return (lhs.type != rhs.type);
}

}; // namespace lexer
}; // namespace pl0c
