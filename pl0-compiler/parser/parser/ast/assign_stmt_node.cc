#include "parser/ast/assign_stmt_node.hh"

#include "parser/ast/division_expr_node.hh"
#include "parser/ast/id_expr_node.hh"
#include "parser/ast/input_expr_node.hh"
#include "parser/ast/int_expr_node.hh"
#include "parser/ast/minus_expr_node.hh"
#include "parser/ast/multiplication_expr_node.hh"
#include "parser/ast/negative_expr_node.hh"
#include "parser/ast/plus_expr_node.hh"

namespace pl0c
{
namespace parser
{

AssignStmtNode::AssignStmtNode(const std::string &lvalueId,
                               const std::shared_ptr<ExprNode> exprNode)
    : lvalueId_{lvalueId}, exprNode_{exprNode}
{
}

auto AssignStmtNode::getExprNode() const -> std::shared_ptr<ExprNode>
{
  return this->exprNode_;
}

auto AssignStmtNode::getLValueId() const -> std::string
{
  return this->lvalueId_;
}

auto AssignStmtNode::getType() const -> StmtNodeType
{
  return StmtNodeType::ASSIGN_STMT;
}

bool operator==(const AssignStmtNode &lhs, const AssignStmtNode &rhs)
{
  const bool sameLValueId = (lhs.lvalueId_ == rhs.lvalueId_);
  const bool sameExprNode = [&lhs, &rhs]() {
    if (lhs.exprNode_->getType() != rhs.exprNode_->getType())
      return false;

    switch (lhs.exprNode_->getType())
    {
    case ExprNodeType::DIVISION_EXPR: {
      const auto lhsDivisionExpr =
          std::dynamic_pointer_cast<DivisionExprNode>(lhs.exprNode_);
      const auto rhsDivisionExpr =
          std::dynamic_pointer_cast<DivisionExprNode>(rhs.exprNode_);

      return (*lhsDivisionExpr == *rhsDivisionExpr);
    }
    case ExprNodeType::ID_EXPR: {
      const auto lhsIdExpr =
          std::dynamic_pointer_cast<IdExprNode>(lhs.exprNode_);
      const auto rhsIdExpr =
          std::dynamic_pointer_cast<IdExprNode>(rhs.exprNode_);

      return (*lhsIdExpr == *rhsIdExpr);
    }
    case ExprNodeType::INT_EXPR: {
      const auto lhsIntExpr =
          std::dynamic_pointer_cast<IntExprNode>(lhs.exprNode_);
      const auto rhsIntExpr =
          std::dynamic_pointer_cast<IntExprNode>(rhs.exprNode_);

      return (*lhsIntExpr == *rhsIntExpr);
    }
    case ExprNodeType::INPUT_EXPR: {
      const auto lhsInputExpr =
          std::dynamic_pointer_cast<InputExprNode>(lhs.exprNode_);
      const auto rhsInputExpr =
          std::dynamic_pointer_cast<InputExprNode>(rhs.exprNode_);

      return (*lhsInputExpr == *rhsInputExpr);
    }
    case ExprNodeType::MINUS_EXPR: {
      const auto lhsMinusExpr =
          std::dynamic_pointer_cast<MinusExprNode>(lhs.exprNode_);
      const auto rhsMinusExpr =
          std::dynamic_pointer_cast<MinusExprNode>(rhs.exprNode_);

      return (*lhsMinusExpr == *rhsMinusExpr);
    }
    case ExprNodeType::MULTIPLICATION_EXPR: {
      const auto lhsMultiplicationExpr =
          std::dynamic_pointer_cast<MultiplicationExprNode>(lhs.exprNode_);
      const auto rhsMultiplicationExpr =
          std::dynamic_pointer_cast<MultiplicationExprNode>(rhs.exprNode_);

      return (*lhsMultiplicationExpr == *rhsMultiplicationExpr);
    }
    case ExprNodeType::NEGATIVE_EXPR: {
      const auto lhsNegativeExpr =
          std::dynamic_pointer_cast<NegativeExprNode>(lhs.exprNode_);
      const auto rhsNegativeExpr =
          std::dynamic_pointer_cast<NegativeExprNode>(rhs.exprNode_);

      return (*lhsNegativeExpr == *rhsNegativeExpr);
    }
    case ExprNodeType::PLUS_EXPR: {
      const auto lhsPlusExpr =
          std::dynamic_pointer_cast<PlusExprNode>(lhs.exprNode_);
      const auto rhsPlusExpr =
          std::dynamic_pointer_cast<PlusExprNode>(rhs.exprNode_);

      return (*lhsPlusExpr == *rhsPlusExpr);
    }
    default: {
      return false;
    }
    }
  }();

  return (sameLValueId && sameExprNode);
}

bool operator!=(const AssignStmtNode &lhs, const AssignStmtNode &rhs)
{
  return !(rhs == lhs);
}

}; // namespace parser
}; // namespace pl0c
