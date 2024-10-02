#include "lexer/token/integer_token.hh"

namespace pl0c
{
namespace lexer
{

IntegerToken::IntegerToken(const std::string integerString)
    : Token(TokenType::INT_LITERAL), integerString_{integerString}
{
}

auto IntegerToken::getInt() const -> std::int64_t
{
  return std::atoi(integerString_.c_str());
}

auto IntegerToken::getIntegerString() const -> std::string
{
  return integerString_;
}

auto IntegerToken::toString() const -> std::string
{
  return std::string{"IntegerToken(" + this->integerString_ + ")"};
}

bool operator==(const IntegerToken &lhs, const IntegerToken &rhs)
{
  return (lhs.integerString_ == rhs.integerString_);
}

bool operator!=(const IntegerToken &lhs, const IntegerToken &rhs)
{
  return (lhs.integerString_ != rhs.integerString_);
}

}; // namespace lexer
}; // namespace pl0c
