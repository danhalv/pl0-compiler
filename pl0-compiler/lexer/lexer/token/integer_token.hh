#pragma once

#include "lexer/token/token.hh"
#include "lexer/token/token_type.hh"

#include <cstdint>
#include <string>

namespace pl0c
{
namespace lexer
{

class IntegerToken : public Token
{
public:
  IntegerToken() = delete;
  IntegerToken(const std::string integerString);

  auto getInt() const -> std::int64_t;
  auto getIntegerString() const -> std::string;

  friend bool operator==(const IntegerToken &, const IntegerToken &);
  friend bool operator!=(const IntegerToken &, const IntegerToken &);

private:
  const std::string integerString_;
};

bool operator==(const IntegerToken &, const IntegerToken &);
bool operator!=(const IntegerToken &, const IntegerToken &);

}; // namespace lexer
}; // namespace pl0c
