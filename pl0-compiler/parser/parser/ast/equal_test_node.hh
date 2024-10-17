#pragma once

#include "parser/ast/expr_node.hh"
#include "parser/ast/test_node.hh"

#include <memory>

namespace pl0c
{
namespace parser
{

class EqualTestNode : public TestNode
{
public:
  EqualTestNode() = delete;
  EqualTestNode(const std::shared_ptr<ExprNode> lhsExprNode,
                const std::shared_ptr<ExprNode> rhsExprNode);

  auto getLhsExprNode() const -> std::shared_ptr<ExprNode>;
  auto getRhsExprNode() const -> std::shared_ptr<ExprNode>;
  auto getType() const -> TestNodeType override;

  friend bool operator==(const EqualTestNode &, const EqualTestNode &);
  friend bool operator!=(const EqualTestNode &, const EqualTestNode &);

private:
  const std::shared_ptr<ExprNode> lhsExprNode_;
  const std::shared_ptr<ExprNode> rhsExprNode_;
};

bool operator==(const EqualTestNode &, const EqualTestNode &);
bool operator!=(const EqualTestNode &, const EqualTestNode &);

}; // namespace parser
}; // namespace pl0c
