#pragma once

#include "lexer/token/token_type.hh"

namespace pl0c
{
namespace lexer
{

struct Token
{
  Token() = delete;
  Token(const TokenType);

  friend bool operator==(const Token &, const Token &);
  friend bool operator!=(const Token &, const Token &);

  const TokenType type;
};

bool operator==(const Token &, const Token &);
bool operator!=(const Token &, const Token &);

}; // namespace lexer
}; // namespace pl0c
