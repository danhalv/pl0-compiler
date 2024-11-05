#include "parser/ast/int_expr_node.hh"

namespace pl0c
{
namespace parser
{

IntExprNode::IntExprNode(const std::int64_t value) : value_{value}
{
}

auto IntExprNode::getType() const -> ExprNodeType
{
  return ExprNodeType::INT_EXPR;
}

auto IntExprNode::getValue() const -> std::int64_t
{
  return this->value_;
}

bool operator==(const IntExprNode &lhs, const IntExprNode &rhs)
{
  return (lhs.value_ == rhs.value_);
}

bool operator!=(const IntExprNode &lhs, const IntExprNode &rhs)
{
  return !(lhs == rhs);
}

}; // namespace parser
}; // namespace pl0c
