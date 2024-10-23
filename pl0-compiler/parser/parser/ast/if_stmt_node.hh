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

class IfStmtNode : public StmtNode
{
public:
  IfStmtNode() = delete;
  IfStmtNode(const std::shared_ptr<TestNode>,
             const std::vector<std::shared_ptr<StmtNode>> &);

  auto getStatements() const -> std::vector<std::shared_ptr<StmtNode>>;
  auto getTestNode() const -> std::shared_ptr<TestNode>;
  auto getType() const -> StmtNodeType override;

  friend bool operator==(const IfStmtNode &, const IfStmtNode &);
  friend bool operator!=(const IfStmtNode &, const IfStmtNode &);

private:
  const std::shared_ptr<TestNode> testNode_;
  const std::vector<std::shared_ptr<StmtNode>> statements_;
};

bool operator==(const IfStmtNode &, const IfStmtNode &);
bool operator!=(const IfStmtNode &, const IfStmtNode &);

}; // namespace parser
}; // namespace pl0c
