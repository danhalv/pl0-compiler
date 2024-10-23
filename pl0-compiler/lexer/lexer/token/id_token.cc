#include "lexer/token/id_token.hh"

#include "lexer/token/token.hh"
#include "lexer/token/token_type.hh"

namespace pl0c
{
namespace lexer
{

IdToken::IdToken(const std::string id) : Token(TokenType::ID), id_{id}
{
}

auto IdToken::getId() const -> std::string
{
  return this->id_;
}

auto IdToken::toString() const -> std::string
{
  return std::string{"IdToken(\"" + this->id_ + "\")"};
}

bool operator==(const IdToken &lhs, const IdToken &rhs)
{
  return (lhs.id_ == rhs.id_);
}

bool operator!=(const IdToken &lhs, const IdToken &rhs)
{
  return (lhs.id_ != rhs.id_);
}

}; // namespace lexer
}; // namespace pl0c
