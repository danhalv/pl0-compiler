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
