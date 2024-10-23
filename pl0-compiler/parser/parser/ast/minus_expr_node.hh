#pragma once

#include "parser/ast/expr_node.hh"

#include <memory>

namespace pl0c
{
namespace parser
{

class MinusExprNode : public ExprNode
{
public:
  MinusExprNode() = delete;
  MinusExprNode(const std::shared_ptr<ExprNode>,
                const std::shared_ptr<ExprNode>);

  auto getLhsExpr() const -> std::shared_ptr<ExprNode>;
  auto getRhsExpr() const -> std::shared_ptr<ExprNode>;
  auto getType() const -> ExprNodeType override;

  friend bool operator==(const MinusExprNode &, const MinusExprNode &);
  friend bool operator!=(const MinusExprNode &, const MinusExprNode &);

private:
  const std::shared_ptr<ExprNode> lhsExpr_;
  const std::shared_ptr<ExprNode> rhsExpr_;
};

bool operator==(const MinusExprNode &, const MinusExprNode &);
bool operator!=(const MinusExprNode &, const MinusExprNode &);

}; // namespace parser
}; // namespace pl0c
