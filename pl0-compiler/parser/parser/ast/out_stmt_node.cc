#include "parser/ast/out_stmt_node.hh"

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

OutStmtNode::OutStmtNode(const std::shared_ptr<ExprNode> exprNode)
    : exprNode_{exprNode}
{
}

auto OutStmtNode::getExprNode() const -> std::shared_ptr<ExprNode>
{
  return this->exprNode_;
}

auto OutStmtNode::getType() const -> StmtNodeType
{
  return StmtNodeType::OUT_STMT;
}

bool operator==(const OutStmtNode &lhs, const OutStmtNode &rhs)
{
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
    return isSameExprNode<MultiplicationExprNode>(lhs.exprNode_, rhs.exprNode_);
  case ExprNodeType::NEGATIVE_EXPR:
    return isSameExprNode<NegativeExprNode>(lhs.exprNode_, rhs.exprNode_);
  case ExprNodeType::PLUS_EXPR:
    return isSameExprNode<PlusExprNode>(lhs.exprNode_, rhs.exprNode_);
  default:
    return false;
  }
}

bool operator!=(const OutStmtNode &lhs, const OutStmtNode &rhs)
{
  return !(lhs == rhs);
}

}; // namespace parser
}; // namespace pl0c
