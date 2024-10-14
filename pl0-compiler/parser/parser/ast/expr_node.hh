#pragma once

#include <memory>

namespace pl0c
{
namespace parser
{

enum class ExprNodeType
{
  DIVISION_EXPR,
  ID_EXPR,
  INPUT_EXPR,
  INT_EXPR,
  MINUS_EXPR,
  MULTIPLICATION_EXPR,
  NEGATIVE_EXPR,
  PLUS_EXPR,
};

class ExprNode
{
public:
  virtual ~ExprNode() = default;
  virtual auto getType() const -> ExprNodeType = 0;
};

template <typename NodeT>
auto isSameExprNode(const std::shared_ptr<ExprNode> lhsNode,
                    const std::shared_ptr<ExprNode> rhsNode) -> bool
{
  const auto lhsNodeExpr = std::dynamic_pointer_cast<NodeT>(lhsNode);
  const auto rhsNodeExpr = std::dynamic_pointer_cast<NodeT>(rhsNode);

  return (*lhsNodeExpr == *rhsNodeExpr);
}

}; // namespace parser
}; // namespace pl0c
