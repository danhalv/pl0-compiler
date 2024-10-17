#include "parser/ast/equal_test_node.hh"

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

EqualTestNode::EqualTestNode(const std::shared_ptr<ExprNode> lhsExprNode,
                             const std::shared_ptr<ExprNode> rhsExprNode)
    : lhsExprNode_{lhsExprNode}, rhsExprNode_{rhsExprNode}
{
}

auto EqualTestNode::getLhsExprNode() const -> std::shared_ptr<ExprNode>
{
  return this->lhsExprNode_;
}

auto EqualTestNode::getRhsExprNode() const -> std::shared_ptr<ExprNode>
{
  return this->rhsExprNode_;
}

auto EqualTestNode::getType() const -> TestNodeType
{
  return TestNodeType::EQUAL_TEST;
}

bool operator==(const EqualTestNode &lhs, const EqualTestNode &rhs)
{
  auto sameExpr = [](const std::shared_ptr<ExprNode> lhsExpr,
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

  return (sameExpr(lhs.lhsExprNode_, rhs.lhsExprNode_) &&
          sameExpr(lhs.rhsExprNode_, rhs.rhsExprNode_));
}

bool operator!=(const EqualTestNode &lhs, const EqualTestNode &rhs)
{
  return !(lhs == rhs);
}

}; // namespace parser
}; // namespace pl0c
