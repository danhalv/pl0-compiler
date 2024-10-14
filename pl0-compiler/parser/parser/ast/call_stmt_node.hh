#pragma once

#include "parser/ast/expr_node.hh"
#include "parser/ast/stmt_node.hh"

#include <memory>
#include <string>
#include <vector>

namespace pl0c
{
namespace parser
{

class CallStmtNode : public StmtNode
{
public:
  CallStmtNode() = delete;
  CallStmtNode(const std::string &procId,
               const std::vector<std::shared_ptr<ExprNode>> &arguments);

  auto getArguments() const -> std::vector<std::shared_ptr<ExprNode>>;
  auto getProcId() const -> std::string;
  auto getType() const -> StmtNodeType override;

  friend bool operator==(const CallStmtNode &, const CallStmtNode &);
  friend bool operator!=(const CallStmtNode &, const CallStmtNode &);

private:
  const std::string procId_;
  const std::vector<std::shared_ptr<ExprNode>> arguments_;
};

bool operator==(const CallStmtNode &, const CallStmtNode &);
bool operator!=(const CallStmtNode &, const CallStmtNode &);

}; // namespace parser
}; // namespace pl0c
