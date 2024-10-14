#include "parser/ast/negative_expr_node.hh"

#include "parser/ast/id_expr_node.hh"
#include "parser/ast/input_expr_node.hh"
#include "parser/ast/int_expr_node.hh"

namespace pl0c
{
namespace parser
{

NegativeExprNode::NegativeExprNode(const std::shared_ptr<ExprNode> exprNode)
    : exprNode_{exprNode}
{
}

auto NegativeExprNode::getExprNode() const -> std::shared_ptr<ExprNode>
{
  return this->exprNode_;
}

auto NegativeExprNode::getType() const -> ExprNodeType
{
  return ExprNodeType::NEGATIVE_EXPR;
}

bool operator==(const NegativeExprNode &lhs, const NegativeExprNode &rhs)
{
  if (lhs.getExprNode()->getType() != rhs.getExprNode()->getType())
    return false;

  switch (lhs.getExprNode()->getType())
  {
  case ExprNodeType::ID_EXPR: {
    const auto lhsIdExpr =
        std::dynamic_pointer_cast<IdExprNode>(lhs.getExprNode());
    const auto rhsIdExpr =
        std::dynamic_pointer_cast<IdExprNode>(rhs.getExprNode());

    return (*lhsIdExpr == *rhsIdExpr);
  }
  case ExprNodeType::INPUT_EXPR: {
    const auto lhsInputExpr =
        std::dynamic_pointer_cast<InputExprNode>(lhs.getExprNode());
    const auto rhsInputExpr =
        std::dynamic_pointer_cast<InputExprNode>(rhs.getExprNode());

    return (*lhsInputExpr == *rhsInputExpr);
  }
  case ExprNodeType::INT_EXPR: {
    const auto lhsIntExpr =
        std::dynamic_pointer_cast<IntExprNode>(lhs.getExprNode());
    const auto rhsIntExpr =
        std::dynamic_pointer_cast<IntExprNode>(rhs.getExprNode());

    return (*lhsIntExpr == *rhsIntExpr);
  }
  default: {
    break;
  }
  }

  return false;
}

bool operator!=(const NegativeExprNode &lhs, const NegativeExprNode &rhs)
{
  return !(lhs == rhs);
}

}; // namespace parser
}; // namespace pl0c
