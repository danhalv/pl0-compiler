#pragma once

#include "parser/ast/expr_node.hh"

#include <string>

namespace pl0c
{
namespace parser
{

class IdExprNode : public ExprNode
{
public:
  IdExprNode() = delete;
  IdExprNode(const std::string id);

  auto getId() const -> std::string;
  auto getType() const -> ExprNodeType override;

  friend bool operator==(const IdExprNode &, const IdExprNode &);
  friend bool operator!=(const IdExprNode &, const IdExprNode &);

private:
  const std::string id_;
};

bool operator==(const IdExprNode &, const IdExprNode &);
bool operator!=(const IdExprNode &, const IdExprNode &);

}; // namespace parser
}; // namespace pl0c
