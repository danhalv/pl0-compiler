#pragma once

#include "lexer/token/token_type.hh"

namespace pl0c
{
namespace lexer
{

class Token
{
public:
  Token() = delete;
  Token(const TokenType);
  virtual ~Token();

  virtual auto getType() const -> TokenType;

  friend bool operator==(const Token &, const Token &);
  friend bool operator!=(const Token &, const Token &);

private:
  const TokenType type_;
};

bool operator==(const Token &, const Token &);
bool operator!=(const Token &, const Token &);

}; // namespace lexer
}; // namespace pl0c
