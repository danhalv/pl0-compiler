#pragma once

#include "parser/ast/expr_node.hh"
#include "parser/ast/test_node.hh"

#include <memory>

namespace pl0c
{
namespace parser
{

class OddTestNode : public TestNode
{
public:
  OddTestNode() = delete;
  OddTestNode(const std::shared_ptr<ExprNode>);

  auto getExprNode() const -> std::shared_ptr<ExprNode>;
  auto getType() const -> TestNodeType override;

  friend bool operator==(const OddTestNode &, const OddTestNode &);
  friend bool operator!=(const OddTestNode &, const OddTestNode &);

private:
  const std::shared_ptr<ExprNode> exprNode_;
};

bool operator==(const OddTestNode &, const OddTestNode &);
bool operator!=(const OddTestNode &, const OddTestNode &);

}; // namespace parser
}; // namespace pl0c
