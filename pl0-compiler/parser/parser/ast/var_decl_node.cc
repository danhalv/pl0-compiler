#include "parser/ast/var_decl_node.hh"

namespace pl0c
{
namespace parser
{

VarDeclNode::VarDeclNode(const std::string &declId,
                         const std::shared_ptr<lexer::Token> declTypeToken)
    : declId_{declId}, declTypeToken_{declTypeToken}
{
}

auto VarDeclNode::getDeclId() const -> std::string
{
  return this->declId_;
}

auto VarDeclNode::getDeclTypeToken() const -> std::shared_ptr<lexer::Token>
{
  return this->declTypeToken_;
}

auto VarDeclNode::getType() const -> DeclNodeType
{
  return DeclNodeType::VAR_DECL;
}

bool operator==(const VarDeclNode &lhs, const VarDeclNode &rhs)
{
  const auto sameDeclId = (lhs.getDeclId() == rhs.getDeclId());
  const auto sameTypeToken =
      (*lhs.getDeclTypeToken() == *rhs.getDeclTypeToken());

  return (sameDeclId && sameTypeToken);
}

bool operator!=(const VarDeclNode &lhs, const VarDeclNode &rhs)
{
  return !(lhs == rhs);
}

}; // namespace parser
}; // namespace pl0c
