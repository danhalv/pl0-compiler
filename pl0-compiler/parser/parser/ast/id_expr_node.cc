#include "parser/ast/id_expr_node.hh"

namespace pl0c
{
namespace parser
{

IdExprNode::IdExprNode(const std::string id) : id_{id}
{
}

auto IdExprNode::getId() const -> std::string
{
  return this->id_;
}

auto IdExprNode::getType() const -> ExprNodeType
{
  return ExprNodeType::ID_EXPR;
}

bool operator==(const IdExprNode &lhs, const IdExprNode &rhs)
{
  return (lhs.id_ == rhs.id_);
}

bool operator!=(const IdExprNode &lhs, const IdExprNode &rhs)
{
  return !(lhs == rhs);
}

}; // namespace parser
}; // namespace pl0c
