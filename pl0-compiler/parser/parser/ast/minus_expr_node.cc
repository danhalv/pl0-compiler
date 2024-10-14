#include "parser/ast/minus_expr_node.hh"

#include "parser/ast/division_expr_node.hh"
#include "parser/ast/id_expr_node.hh"
#include "parser/ast/input_expr_node.hh"
#include "parser/ast/int_expr_node.hh"
#include "parser/ast/multiplication_expr_node.hh"
#include "parser/ast/negative_expr_node.hh"
#include "parser/ast/plus_expr_node.hh"

namespace pl0c
{
namespace parser
{

MinusExprNode::MinusExprNode(const std::shared_ptr<ExprNode> lhsExpr,
                             const std::shared_ptr<ExprNode> rhsExpr)
    : lhsExpr_{lhsExpr}, rhsExpr_{rhsExpr}
{
}

auto MinusExprNode::getLhsExpr() const -> std::shared_ptr<ExprNode>
{
  return this->lhsExpr_;
}

auto MinusExprNode::getRhsExpr() const -> std::shared_ptr<ExprNode>
{
  return this->rhsExpr_;
}

auto MinusExprNode::getType() const -> ExprNodeType
{
  return ExprNodeType::MINUS_EXPR;
}

bool operator==(const MinusExprNode &lhs, const MinusExprNode &rhs)
{
  auto sameNode = [](const std::shared_ptr<ExprNode> lhsExpr,
                     const std::shared_ptr<ExprNode> rhsExpr) {
    if (lhsExpr->getType() != rhsExpr->getType())
      return false;

    switch (lhsExpr->getType())
    {
    case ExprNodeType::DIVISION_EXPR: {
      const auto lhsDivisionExpr =
          std::dynamic_pointer_cast<DivisionExprNode>(lhsExpr);
      const auto rhsDivisionExpr =
          std::dynamic_pointer_cast<DivisionExprNode>(rhsExpr);

      return (*lhsDivisionExpr == *rhsDivisionExpr);
    }
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
    case ExprNodeType::MINUS_EXPR: {
      const auto lhsMinusExpr =
          std::dynamic_pointer_cast<MinusExprNode>(lhsExpr);
      const auto rhsMinusExpr =
          std::dynamic_pointer_cast<MinusExprNode>(rhsExpr);

      return (*lhsMinusExpr == *rhsMinusExpr);
    }
    case ExprNodeType::MULTIPLICATION_EXPR: {
      const auto lhsMultiplicationExpr =
          std::dynamic_pointer_cast<MultiplicationExprNode>(lhsExpr);
      const auto rhsMultiplicationExpr =
          std::dynamic_pointer_cast<MultiplicationExprNode>(rhsExpr);

      return (*lhsMultiplicationExpr == *rhsMultiplicationExpr);
    }
    case ExprNodeType::NEGATIVE_EXPR: {
      const auto lhsNegativeExpr =
          std::dynamic_pointer_cast<NegativeExprNode>(lhsExpr);
      const auto rhsNegativeExpr =
          std::dynamic_pointer_cast<NegativeExprNode>(rhsExpr);

      return (*lhsNegativeExpr == *rhsNegativeExpr);
    }
    case ExprNodeType::PLUS_EXPR: {
      const auto lhsPlusExpr =
          std::dynamic_pointer_cast<MinusExprNode>(lhsExpr);
      const auto rhsPlusExpr =
          std::dynamic_pointer_cast<MinusExprNode>(rhsExpr);

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

bool operator!=(const MinusExprNode &lhs, const MinusExprNode &rhs)
{
  return !(lhs == rhs);
}

}; // namespace parser
}; // namespace pl0c
