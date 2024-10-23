#pragma once

#include "parser/ast/expr_node.hh"
#include "parser/ast/test_node.hh"

#include <memory>

namespace pl0c
{
namespace parser
{

class ComparisonTestNode : public TestNode
{
public:
  ComparisonTestNode() = delete;
  ComparisonTestNode(const TestNodeType,
                     const std::shared_ptr<ExprNode> lhsExprNode,
                     const std::shared_ptr<ExprNode> rhsExprNode);

  auto getLhsExprNode() const -> std::shared_ptr<ExprNode>;
  auto getRhsExprNode() const -> std::shared_ptr<ExprNode>;
  auto getType() const -> TestNodeType override;

  friend bool operator==(const ComparisonTestNode &,
                         const ComparisonTestNode &);
  friend bool operator!=(const ComparisonTestNode &,
                         const ComparisonTestNode &);

private:
  const TestNodeType testNodeType_;
  const std::shared_ptr<ExprNode> lhsExprNode_;
  const std::shared_ptr<ExprNode> rhsExprNode_;
};

bool operator==(const ComparisonTestNode &, const ComparisonTestNode &);
bool operator!=(const ComparisonTestNode &, const ComparisonTestNode &);

}; // namespace parser
}; // namespace pl0c
