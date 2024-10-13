#include "parser/ast/input_expr_node.hh"

namespace pl0c
{
namespace parser
{

InputExprNode::InputExprNode()
{
}

auto InputExprNode::getType() const -> ExprNodeType
{
  return ExprNodeType::INPUT_EXPR;
}

bool operator==(const InputExprNode &, const InputExprNode &)
{
  return true;
}

bool operator!=(const InputExprNode &lhs, const InputExprNode &rhs)
{
  return !(lhs == rhs);
}

}; // namespace parser
}; // namespace pl0c
