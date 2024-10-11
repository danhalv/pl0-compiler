#include "parser/ast/assign_stmt_node.hh"

#include "parser/ast/int_expr_node.hh"

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
    case ExprNodeType::INT_EXPR: {
      const auto lhsIntExpr =
          std::dynamic_pointer_cast<IntExprNode>(lhs.exprNode_);
      const auto rhsIntExpr =
          std::dynamic_pointer_cast<IntExprNode>(rhs.exprNode_);

      return (*lhsIntExpr == *rhsIntExpr);
    }
    default: {
      break;
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
