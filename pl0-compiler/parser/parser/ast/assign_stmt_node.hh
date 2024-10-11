#pragma once

#include "parser/ast/expr_node.hh"
#include "parser/ast/stmt_node.hh"

#include <memory>
#include <string>

namespace pl0c
{
namespace parser
{

class AssignStmtNode : public StmtNode
{
public:
  AssignStmtNode() = delete;
  AssignStmtNode(const std::string &lvalueId,
                 const std::shared_ptr<ExprNode> exprNode);

  auto getExprNode() const -> std::shared_ptr<ExprNode>;
  auto getLValueId() const -> std::string;
  auto getType() const -> StmtNodeType override;

  friend bool operator==(const AssignStmtNode &, const AssignStmtNode &);
  friend bool operator!=(const AssignStmtNode &, const AssignStmtNode &);

private:
  const std::string lvalueId_;
  const std::shared_ptr<ExprNode> exprNode_;
};

bool operator==(const AssignStmtNode &, const AssignStmtNode &);
bool operator!=(const AssignStmtNode &, const AssignStmtNode &);

}; // namespace parser
}; // namespace pl0c
