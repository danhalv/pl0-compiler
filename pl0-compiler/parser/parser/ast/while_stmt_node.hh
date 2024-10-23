#pragma once

#include "parser/ast/expr_node.hh"
#include "parser/ast/stmt_node.hh"
#include "parser/ast/test_node.hh"

#include <memory>
#include <vector>

namespace pl0c
{
namespace parser
{

class WhileStmtNode : public StmtNode
{
public:
  WhileStmtNode() = delete;
  WhileStmtNode(const std::shared_ptr<TestNode>,
                const std::vector<std::shared_ptr<StmtNode>> &);

  auto getStatements() const -> std::vector<std::shared_ptr<StmtNode>>;
  auto getTestNode() const -> std::shared_ptr<TestNode>;
  auto getType() const -> StmtNodeType override;

  friend bool operator==(const WhileStmtNode &, const WhileStmtNode &);
  friend bool operator!=(const WhileStmtNode &, const WhileStmtNode &);

private:
  const std::shared_ptr<TestNode> testNode_;
  const std::vector<std::shared_ptr<StmtNode>> statements_;
};

bool operator==(const WhileStmtNode &, const WhileStmtNode &);
bool operator!=(const WhileStmtNode &, const WhileStmtNode &);

}; // namespace parser
}; // namespace pl0c
