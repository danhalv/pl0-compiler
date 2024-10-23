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
    case ExprNodeType::DIVISION_EXPR:
      return isSameExprNode<DivisionExprNode>(lhs.exprNode_, rhs.exprNode_);
    case ExprNodeType::ID_EXPR:
      return isSameExprNode<IdExprNode>(lhs.exprNode_, rhs.exprNode_);
    case ExprNodeType::INT_EXPR:
      return isSameExprNode<IntExprNode>(lhs.exprNode_, rhs.exprNode_);
    case ExprNodeType::INPUT_EXPR:
      return isSameExprNode<InputExprNode>(lhs.exprNode_, rhs.exprNode_);
    case ExprNodeType::MINUS_EXPR:
      return isSameExprNode<MinusExprNode>(lhs.exprNode_, rhs.exprNode_);
    case ExprNodeType::MULTIPLICATION_EXPR:
      return isSameExprNode<MultiplicationExprNode>(lhs.exprNode_,
                                                    rhs.exprNode_);
    case ExprNodeType::NEGATIVE_EXPR:
      return isSameExprNode<NegativeExprNode>(lhs.exprNode_, rhs.exprNode_);
    case ExprNodeType::PLUS_EXPR:
      return isSameExprNode<PlusExprNode>(lhs.exprNode_, rhs.exprNode_);
    default:
      return false;
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
