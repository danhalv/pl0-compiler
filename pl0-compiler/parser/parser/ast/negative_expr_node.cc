#include "parser/ast/negative_expr_node.hh"

#include "parser/ast/division_expr_node.hh"
#include "parser/ast/id_expr_node.hh"
#include "parser/ast/input_expr_node.hh"
#include "parser/ast/int_expr_node.hh"
#include "parser/ast/minus_expr_node.hh"
#include "parser/ast/multiplication_expr_node.hh"
#include "parser/ast/plus_expr_node.hh"

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
  case ExprNodeType::DIVISION_EXPR:
    return isSameExprNode<DivisionExprNode>(lhs.getExprNode(),
                                            rhs.getExprNode());
  case ExprNodeType::ID_EXPR:
    return isSameExprNode<IdExprNode>(lhs.getExprNode(), rhs.getExprNode());
  case ExprNodeType::INT_EXPR:
    return isSameExprNode<IntExprNode>(lhs.getExprNode(), rhs.getExprNode());
  case ExprNodeType::INPUT_EXPR:
    return isSameExprNode<InputExprNode>(lhs.getExprNode(), rhs.getExprNode());
  case ExprNodeType::MINUS_EXPR:
    return isSameExprNode<MinusExprNode>(lhs.getExprNode(), rhs.getExprNode());
  case ExprNodeType::MULTIPLICATION_EXPR:
    return isSameExprNode<MultiplicationExprNode>(lhs.getExprNode(),
                                                  rhs.getExprNode());
  case ExprNodeType::NEGATIVE_EXPR:
    return isSameExprNode<NegativeExprNode>(lhs.getExprNode(),
                                            rhs.getExprNode());
  case ExprNodeType::PLUS_EXPR:
    return isSameExprNode<PlusExprNode>(lhs.getExprNode(), rhs.getExprNode());
  default:
    return false;
  }

  return false;
}

bool operator!=(const NegativeExprNode &lhs, const NegativeExprNode &rhs)
{
  return !(lhs == rhs);
}

}; // namespace parser
}; // namespace pl0c
