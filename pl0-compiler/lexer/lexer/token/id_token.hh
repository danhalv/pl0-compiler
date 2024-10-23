#pragma once

#include "lexer/token/token.hh"
#include "lexer/token/token_type.hh"

#include <string>

namespace pl0c
{
namespace lexer
{

class IdToken : public Token
{
public:
  IdToken() = delete;
  IdToken(const std::string id);

  auto getId() const -> std::string;
  auto toString() const -> std::string override;

  friend bool operator==(const IdToken &, const IdToken &);
  friend bool operator!=(const IdToken &, const IdToken &);

private:
  const std::string id_;
};

bool operator==(const IdToken &, const IdToken &);
bool operator!=(const IdToken &, const IdToken &);

}; // namespace lexer
}; // namespace pl0c
