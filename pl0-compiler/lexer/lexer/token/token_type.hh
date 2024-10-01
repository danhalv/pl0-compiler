#pragma once

namespace pl0c
{
namespace lexer
{

enum class TokenType
{
  // literals
  ID,
  INTEGER,
  // end of literals
  // punctuation
  COLON,
  COMMA,
  DOT,
  LEFT_PAREN,
  RIGHT_PAREN,
  SEMI_COLON,
  // end of punctuation
};

}; // namespace lexer
}; // namespace pl0c
