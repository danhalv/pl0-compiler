#include "parser/ast/plus_expr_node.hh"

#include "parser/ast/id_expr_node.hh"
#include "parser/ast/input_expr_node.hh"
#include "parser/ast/int_expr_node.hh"

namespace pl0c
{
namespace parser
{

PlusExprNode::PlusExprNode(const std::shared_ptr<ExprNode> &lhsExpr,
                           const std::shared_ptr<ExprNode> &rhsExpr)
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
    case ExprNodeType::ID_EXPR: {
      const auto lhsIdExpr = std::dynamic_pointer_cast<IdExprNode>(lhsExpr);
      const auto rhsIdExpr = std::dynamic_pointer_cast<IdExprNode>(rhsExpr);

      return (*lhsIdExpr == *rhsIdExpr);
    }
    case ExprNodeType::INT_EXPR: {
      const auto lhsIntExpr = std::dynamic_pointer_cast<IntExprNode>(lhsExpr);
      const auto rhsIntExpr = std::dynamic_pointer_cast<IntExprNode>(rhsExpr);

      return (*lhsIntExpr == *rhsIntExpr);
    }
    case ExprNodeType::INPUT_EXPR: {
      const auto lhsInputExpr =
          std::dynamic_pointer_cast<InputExprNode>(lhsExpr);
      const auto rhsInputExpr =
          std::dynamic_pointer_cast<InputExprNode>(rhsExpr);

      return (*lhsInputExpr == *rhsInputExpr);
    }
    case ExprNodeType::PLUS_EXPR: {
      const auto lhsPlusExpr = std::dynamic_pointer_cast<PlusExprNode>(lhsExpr);
      const auto rhsPlusExpr = std::dynamic_pointer_cast<PlusExprNode>(rhsExpr);

      return (*lhsPlusExpr == *rhsPlusExpr);
    }
    default: {
      return false;
    }
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
