#pragma once

#include "lexer/token/token.hh"
#include "parser/ast/decl_node.hh"

#include <memory>
#include <string>

namespace pl0c
{
namespace parser
{

class ConstDeclNode : public DeclNode
{
public:
  ConstDeclNode() = delete;
  ConstDeclNode(const std::string &declId,
                const std::shared_ptr<lexer::Token> &declTypeToken,
                const std::shared_ptr<lexer::Token> &declExprToken);
  ConstDeclNode(const ConstDeclNode &) = delete;
  ConstDeclNode &operator=(const DeclNodeType &) = delete;

  auto getDeclId() const -> std::string;
  auto getDeclTypeToken() const -> std::shared_ptr<lexer::Token>;
  auto getDeclExprToken() const -> std::shared_ptr<lexer::Token>;
  auto getType() const -> DeclNodeType override;

  friend bool operator==(const ConstDeclNode &, const ConstDeclNode &);
  friend bool operator!=(const ConstDeclNode &, const ConstDeclNode &);

private:
  const std::string declId_;
  const std::shared_ptr<lexer::Token> declTypeToken_;
  const std::shared_ptr<lexer::Token> declExprToken_;
};

bool operator==(const ConstDeclNode &, const ConstDeclNode &);
bool operator!=(const ConstDeclNode &, const ConstDeclNode &);

}; // namespace parser
}; // namespace pl0c
