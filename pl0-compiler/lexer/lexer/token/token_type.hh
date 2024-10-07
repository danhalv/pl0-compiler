#pragma once

#include <map>
#include <string>

namespace pl0c
{
namespace lexer
{

enum class TokenType
{
  WHITESPACE,
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

const auto tokenTypeNameMap = std::map<TokenType, std::string>{
    {TokenType::WHITESPACE, "WHITESPACE"},
    {TokenType::ID, "ID"},
    {TokenType::INT_LITERAL, "INT_LITERAL"},
    {TokenType::MODULE, "MODULE"},
    {TokenType::PROCEDURE, "PROCEDURE"},
    {TokenType::BEGIN, "BEGIN"},
    {TokenType::END, "END"},
    {TokenType::CONST, "CONST"},
    {TokenType::VAR, "VAR"},
    {TokenType::INT, "INT"},
    {TokenType::IF, "IF"},
    {TokenType::THEN, "THEN"},
    {TokenType::WHILE, "WHILE"},
    {TokenType::DO, "DO"},
    {TokenType::INPUT, "INPUT"},
    {TokenType::OUTPUT, "OUTPUT"},
    {TokenType::ODD, "ODD"},
    {TokenType::WALRUS, "WALRUS"},
    {TokenType::ASTERISK, "ASTERISK"},
    {TokenType::SLASH, "SLASH"},
    {TokenType::PLUS, "PLUS"},
    {TokenType::MINUS, "MINUS"},
    {TokenType::ASSIGNMENT, "ASSIGNMENT"},
    {TokenType::DIAMOND, "DIAMOND"},
    {TokenType::LEQ, "LEQ"},
    {TokenType::LE, "LE"},
    {TokenType::GEQ, "GEQ"},
    {TokenType::GE, "GE"},
    {TokenType::COLON, "COLON"},
    {TokenType::COMMA, "COMMA"},
    {TokenType::DOT, "DOT"},
    {TokenType::LEFT_PAREN, "LEFT_PAREN"},
    {TokenType::RIGHT_PAREN, "RIGHT_PAREN"},
    {TokenType::SEMI_COLON, "SEMI_COLON"},
};

}; // namespace lexer
}; // namespace pl0c
