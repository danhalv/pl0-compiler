#pragma once

#include "parser/ast/expr_node.hh"

#include <memory>

namespace pl0c
{
namespace parser
{

class NegativeExprNode : public ExprNode
{
public:
  NegativeExprNode() = delete;
  NegativeExprNode(const std::shared_ptr<ExprNode>);

  auto getExprNode() const -> std::shared_ptr<ExprNode>;
  auto getType() const -> ExprNodeType override;

  friend bool operator==(const NegativeExprNode &, const NegativeExprNode &);
  friend bool operator!=(const NegativeExprNode &, const NegativeExprNode &);

private:
  const std::shared_ptr<ExprNode> exprNode_;
};

bool operator==(const NegativeExprNode &, const NegativeExprNode &);
bool operator!=(const NegativeExprNode &, const NegativeExprNode &);

}; // namespace parser
}; // namespace pl0c
