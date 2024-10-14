#include "parser/ast/call_stmt_node.hh"

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

CallStmtNode::CallStmtNode(
    const std::string &procId,
    const std::vector<std::shared_ptr<ExprNode>> &arguments)
    : procId_{procId}, arguments_{arguments}
{
}

auto CallStmtNode::getArguments() const
    -> std::vector<std::shared_ptr<ExprNode>>
{
  return this->arguments_;
}

auto CallStmtNode::getProcId() const -> std::string
{
  return this->procId_;
}

auto CallStmtNode::getType() const -> StmtNodeType
{
  return StmtNodeType::CALL_STMT;
}

bool operator==([[maybe_unused]] const CallStmtNode &lhs,
                [[maybe_unused]] const CallStmtNode &rhs)
{
  const bool sameProcId = (lhs.procId_ == rhs.procId_);

  const bool sameArguments = [&lhs, &rhs]() {
    if (lhs.arguments_.size() != rhs.arguments_.size())
      return false;

    auto sameArgument = [](const std::shared_ptr<ExprNode> lhsExpr,
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
            std::dynamic_pointer_cast<PlusExprNode>(lhsExpr);
        const auto rhsPlusExpr =
            std::dynamic_pointer_cast<PlusExprNode>(rhsExpr);

        return (*lhsPlusExpr == *rhsPlusExpr);
      }
      default: {
        return false;
      }
      }
    };

    for (std::size_t i = 0; i < lhs.arguments_.size(); ++i)
    {
      if (!sameArgument(lhs.arguments_.at(i), rhs.arguments_.at(i)))
        return false;
    }

    return true;
  }();

  return (sameProcId && sameArguments);
}

bool operator!=(const CallStmtNode &lhs, const CallStmtNode &rhs)
{
  return !(lhs == rhs);
}

}; // namespace parser
}; // namespace pl0c
