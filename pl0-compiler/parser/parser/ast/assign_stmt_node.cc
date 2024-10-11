#include "parser/ast/assign_stmt_node.hh"

namespace pl0c
{
namespace parser
{

AssignStmtNode::AssignStmtNode(const std::string &lvalueId,
                               const std::shared_ptr<ExprNode> exprNode)
    : lvalueId_{lvalueId}, exprNode_{exprNode}
{
}

auto AssignStmtNode::getExprNode() const -> std::shared_ptr<ExprNode>
{
  return this->exprNode_;
}

auto AssignStmtNode::getLValueId() const -> std::string
{
  return this->lvalueId_;
}

auto AssignStmtNode::getType() const -> StmtNodeType
{
  return StmtNodeType::ASSIGN_STMT;
}

bool operator==(const AssignStmtNode &lhs, const AssignStmtNode &rhs)
{
  const bool sameLValueId = (lhs.lvalueId_ == rhs.lvalueId_);
  const bool sameExprNode = true; // TODO: update

  return (sameLValueId && sameExprNode);
}

bool operator!=(const AssignStmtNode &lhs, const AssignStmtNode &rhs)
{
  return !(rhs == lhs);
}

}; // namespace parser
}; // namespace pl0c
