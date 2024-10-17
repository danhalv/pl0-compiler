#pragma once

#include "parser/ast/expr_node.hh"
#include "parser/ast/test_node.hh"

#include <memory>

namespace pl0c
{
namespace parser
{

class NotEqualTestNode : public TestNode
{
public:
  NotEqualTestNode() = delete;
  NotEqualTestNode(const std::shared_ptr<ExprNode> lhsExprNode,
                   const std::shared_ptr<ExprNode> rhsExprNode);

  auto getLhsExprNode() const -> std::shared_ptr<ExprNode>;
  auto getRhsExprNode() const -> std::shared_ptr<ExprNode>;
  auto getType() const -> TestNodeType override;

  friend bool operator==(const NotEqualTestNode &, const NotEqualTestNode &);
  friend bool operator!=(const NotEqualTestNode &, const NotEqualTestNode &);

private:
  const std::shared_ptr<ExprNode> lhsExprNode_;
  const std::shared_ptr<ExprNode> rhsExprNode_;
};

bool operator==(const NotEqualTestNode &, const NotEqualTestNode &);
bool operator!=(const NotEqualTestNode &, const NotEqualTestNode &);

}; // namespace parser
}; // namespace pl0c
