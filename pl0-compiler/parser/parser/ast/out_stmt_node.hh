#pragma once

#include "parser/ast/expr_node.hh"
#include "parser/ast/stmt_node.hh"

#include <memory>

namespace pl0c
{
namespace parser
{

class OutStmtNode : public StmtNode
{
public:
  OutStmtNode() = delete;
  OutStmtNode(const std::shared_ptr<ExprNode> exprNode);

  auto getExprNode() const -> std::shared_ptr<ExprNode>;
  auto getType() const -> StmtNodeType override;

  friend bool operator==(const OutStmtNode &, const OutStmtNode &);
  friend bool operator!=(const OutStmtNode &, const OutStmtNode &);

private:
  const std::shared_ptr<ExprNode> exprNode_;
};

bool operator==(const OutStmtNode &, const OutStmtNode &);
bool operator!=(const OutStmtNode &, const OutStmtNode &);

}; // namespace parser
}; // namespace pl0c
