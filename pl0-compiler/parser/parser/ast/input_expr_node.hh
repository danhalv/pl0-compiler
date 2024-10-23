#pragma once

#include "parser/ast/expr_node.hh"

namespace pl0c
{
namespace parser
{

class InputExprNode : public ExprNode
{
public:
  InputExprNode();

  auto getType() const -> ExprNodeType override;

  friend bool operator==(const InputExprNode &, const InputExprNode &);
  friend bool operator!=(const InputExprNode &, const InputExprNode &);
};

bool operator==(const InputExprNode &, const InputExprNode &);
bool operator!=(const InputExprNode &, const InputExprNode &);

}; // namespace parser
}; // namespace pl0c
