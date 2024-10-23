#include "parser/ast/plus_expr_node.hh"

#include "parser/ast/division_expr_node.hh"
#include "parser/ast/id_expr_node.hh"
#include "parser/ast/input_expr_node.hh"
#include "parser/ast/int_expr_node.hh"
#include "parser/ast/minus_expr_node.hh"
#include "parser/ast/multiplication_expr_node.hh"
#include "parser/ast/negative_expr_node.hh"

namespace pl0c
{
namespace parser
{

PlusExprNode::PlusExprNode(const std::shared_ptr<ExprNode> lhsExpr,
                           const std::shared_ptr<ExprNode> rhsExpr)
    : lhsExpr_{lhsExpr}, rhsExpr_{rhsExpr}
{
}

auto PlusExprNode::getLhsExpr() const -> std::shared_ptr<ExprNode>
{
  return this->lhsExpr_;
}

auto PlusExprNode::getRhsExpr() const -> std::shared_ptr<ExprNode>
{
  return this->rhsExpr_;
}

auto PlusExprNode::getType() const -> ExprNodeType
{
  return ExprNodeType::PLUS_EXPR;
}

bool operator==(const PlusExprNode &lhs, const PlusExprNode &rhs)
{
  auto sameNode = [](const std::shared_ptr<ExprNode> lhsExpr,
                     const std::shared_ptr<ExprNode> rhsExpr) {
    if (lhsExpr->getType() != rhsExpr->getType())
      return false;

    switch (lhsExpr->getType())
    {
    case ExprNodeType::DIVISION_EXPR:
      return isSameExprNode<DivisionExprNode>(lhsExpr, rhsExpr);
    case ExprNodeType::ID_EXPR:
      return isSameExprNode<IdExprNode>(lhsExpr, rhsExpr);
    case ExprNodeType::INT_EXPR:
      return isSameExprNode<IntExprNode>(lhsExpr, rhsExpr);
    case ExprNodeType::INPUT_EXPR:
      return isSameExprNode<InputExprNode>(lhsExpr, rhsExpr);
    case ExprNodeType::MINUS_EXPR:
      return isSameExprNode<MinusExprNode>(lhsExpr, rhsExpr);
    case ExprNodeType::MULTIPLICATION_EXPR:
      return isSameExprNode<MultiplicationExprNode>(lhsExpr, rhsExpr);
    case ExprNodeType::NEGATIVE_EXPR:
      return isSameExprNode<NegativeExprNode>(lhsExpr, rhsExpr);
    case ExprNodeType::PLUS_EXPR:
      return isSameExprNode<PlusExprNode>(lhsExpr, rhsExpr);
    default:
      return false;
    }
  };

  const bool sameLhsExprs = sameNode(lhs.lhsExpr_, rhs.lhsExpr_);
  const bool sameRhsExprs = sameNode(lhs.rhsExpr_, rhs.rhsExpr_);

  return (sameLhsExprs && sameRhsExprs);
}

bool operator!=(const PlusExprNode &lhs, const PlusExprNode &rhs)
{
  return !(lhs == rhs);
}

}; // namespace parser
}; // namespace pl0c
