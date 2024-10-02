#pragma once

namespace pl0c
{
namespace lexer
{

enum class TokenType
{
  // literals
  ID,
  INT_LITERAL,
  // end of literals
  // keywords
  MODULE,
  PROCEDURE,
  BEGIN,
  END,
  CONST,
  VAR,
  INT,
  IF,
  THEN,
  WHILE,
  DO,
  INPUT,
  OUTPUT,
  ODD,
  // end of keywords
  // operators
  WALRUS,
  ASTERISK,
  SLASH,
  PLUS,
  MINUS,
  ASSIGNMENT,
  DIAMOND,
  LEQ,
  LE,
  GEQ,
  GE,
  // end of operators
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
