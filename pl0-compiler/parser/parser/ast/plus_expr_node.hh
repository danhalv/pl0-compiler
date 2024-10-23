#pragma once

#include "parser/ast/expr_node.hh"

#include <memory>

namespace pl0c
{
namespace parser
{

class PlusExprNode : public ExprNode
{
public:
  PlusExprNode() = delete;
  PlusExprNode(const std::shared_ptr<ExprNode>,
               const std::shared_ptr<ExprNode>);

  auto getLhsExpr() const -> std::shared_ptr<ExprNode>;
  auto getRhsExpr() const -> std::shared_ptr<ExprNode>;
  auto getType() const -> ExprNodeType override;

  friend bool operator==(const PlusExprNode &, const PlusExprNode &);
  friend bool operator!=(const PlusExprNode &, const PlusExprNode &);

private:
  const std::shared_ptr<ExprNode> lhsExpr_;
  const std::shared_ptr<ExprNode> rhsExpr_;
};

bool operator==(const PlusExprNode &, const PlusExprNode &);
bool operator!=(const PlusExprNode &, const PlusExprNode &);

}; // namespace parser
}; // namespace pl0c
