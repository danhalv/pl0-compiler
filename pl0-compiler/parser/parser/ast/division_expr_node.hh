#pragma once

#include "parser/ast/expr_node.hh"

#include <memory>

namespace pl0c
{
namespace parser
{

class DivisionExprNode : public ExprNode
{
public:
  DivisionExprNode() = delete;
  DivisionExprNode(const std::shared_ptr<ExprNode>,
                   const std::shared_ptr<ExprNode>);

  auto getLhsExpr() const -> std::shared_ptr<ExprNode>;
  auto getRhsExpr() const -> std::shared_ptr<ExprNode>;
  auto getType() const -> ExprNodeType override;

  friend bool operator==(const DivisionExprNode &, const DivisionExprNode &);
  friend bool operator!=(const DivisionExprNode &, const DivisionExprNode &);

private:
  const std::shared_ptr<ExprNode> lhsExpr_;
  const std::shared_ptr<ExprNode> rhsExpr_;
};

bool operator==(const DivisionExprNode &, const DivisionExprNode &);
bool operator!=(const DivisionExprNode &, const DivisionExprNode &);

}; // namespace parser
}; // namespace pl0c
