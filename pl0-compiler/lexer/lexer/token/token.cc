#include "lexer/token/token.hh"

namespace pl0c
{
namespace lexer
{

Token::Token(const TokenType type) : type_{type}
{
}

Token::~Token()
{
}

auto Token::getType() const -> TokenType
{
  return type_;
}

auto Token::toString() const -> std::string
{
  return std::string{"Token(" + tokenTypeNameMap.at(this->type_) + ")"};
}

bool operator==(const Token &lhs, const Token &rhs)
{
  return (lhs.type_ == rhs.type_);
}

bool operator!=(const Token &lhs, const Token &rhs)
{
  return (lhs.type_ != rhs.type_);
}

}; // namespace lexer
}; // namespace pl0c
