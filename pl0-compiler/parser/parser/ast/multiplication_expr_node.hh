#pragma once

#include "parser/ast/expr_node.hh"

#include <memory>

namespace pl0c
{
namespace parser
{

class MultiplicationExprNode : public ExprNode
{
public:
  MultiplicationExprNode() = delete;
  MultiplicationExprNode(const std::shared_ptr<ExprNode>,
                         const std::shared_ptr<ExprNode>);

  auto getLhsExpr() const -> std::shared_ptr<ExprNode>;
  auto getRhsExpr() const -> std::shared_ptr<ExprNode>;
  auto getType() const -> ExprNodeType override;

  friend bool operator==(const MultiplicationExprNode &,
                         const MultiplicationExprNode &);
  friend bool operator!=(const MultiplicationExprNode &,
                         const MultiplicationExprNode &);

private:
  const std::shared_ptr<ExprNode> lhsExpr_;
  const std::shared_ptr<ExprNode> rhsExpr_;
};

bool operator==(const MultiplicationExprNode &, const MultiplicationExprNode &);
bool operator!=(const MultiplicationExprNode &, const MultiplicationExprNode &);

}; // namespace parser
}; // namespace pl0c
