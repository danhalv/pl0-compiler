#pragma once

#include "lexer/token/id_token.hh"
#include "lexer/token/integer_token.hh"
#include "parser/ast/decl_node.hh"

#include <memory>
#include <string>

namespace pl0c
{
namespace parser
{

class VarDeclNode : public DeclNode
{
public:
  VarDeclNode() = delete;
  VarDeclNode(const std::string &declId,
              const std::shared_ptr<lexer::Token> &declTypeToken);
  VarDeclNode(const VarDeclNode &) = delete;
  VarDeclNode &operator=(const DeclNodeType &) = delete;

  auto getDeclId() const -> std::string override;
  auto getDeclTypeToken() const -> std::shared_ptr<lexer::Token>;
  auto getType() const -> DeclNodeType override;

  friend bool operator==(const VarDeclNode &, const VarDeclNode &);
  friend bool operator!=(const VarDeclNode &, const VarDeclNode &);

private:
  const std::string declId_;
  const std::shared_ptr<lexer::Token> declTypeToken_;
};

bool operator==(const VarDeclNode &, const VarDeclNode &);
bool operator!=(const VarDeclNode &, const VarDeclNode &);

}; // namespace parser
}; // namespace pl0c
