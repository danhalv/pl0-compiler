#pragma once

#include "parser/ast/expr_node.hh"

#include <cstdint>

namespace pl0c
{
namespace parser
{

class IntExprNode : public ExprNode
{
public:
  IntExprNode() = delete;
  IntExprNode(const std::int64_t);

  auto getType() const -> ExprNodeType override;
  auto getValue() const -> std::int64_t;

  friend bool operator==(const IntExprNode &, const IntExprNode &);
  friend bool operator!=(const IntExprNode &, const IntExprNode &);

private:
  const std::int64_t value_;
};

bool operator==(const IntExprNode &, const IntExprNode &);
bool operator!=(const IntExprNode &, const IntExprNode &);

}; // namespace parser
}; // namespace pl0c
