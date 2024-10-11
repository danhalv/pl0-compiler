#include "parser/ast/block_node.hh"

#include "parser/ast/assign_stmt_node.hh"
#include "parser/ast/const_decl_node.hh"
#include "parser/ast/proc_decl_node.hh"
#include "parser/ast/var_decl_node.hh"

namespace pl0c
{
namespace parser
{

BlockNode::BlockNode(const std::vector<std::shared_ptr<DeclNode>> &declarations,
                     const std::vector<std::shared_ptr<StmtNode>> &statements)
    : declarations_{declarations}, statements_{statements}
{
}

auto BlockNode::getDeclarations() const
    -> std::vector<std::shared_ptr<DeclNode>>
{
  return this->declarations_;
}

auto BlockNode::getStatements() const -> std::vector<std::shared_ptr<StmtNode>>
{
  return this->statements_;
}

bool operator==(const BlockNode &lhs, const BlockNode &rhs)
{
  const bool sameDeclarationList = [&lhs, &rhs]() {
    if (lhs.declarations_.size() != rhs.declarations_.size())
      return false;

    if (lhs.statements_.size() != rhs.statements_.size())
      return false;

    for (std::size_t i = 0; i < lhs.declarations_.size(); ++i)
    {
      if (lhs.declarations_.at(i)->getType() !=
          rhs.declarations_.at(i)->getType())
        return false;

      switch (lhs.declarations_.at(i)->getType())
      {
      case DeclNodeType::CONST_DECL: {
        const auto lhsConstDecl =
            std::dynamic_pointer_cast<ConstDeclNode>(lhs.declarations_.at(i));
        const auto rhsConstDecl =
            std::dynamic_pointer_cast<ConstDeclNode>(rhs.declarations_.at(i));

        if (*lhsConstDecl != *rhsConstDecl)
          return false;

        break;
      }
      case DeclNodeType::PROC_DECL: {
        const auto lhsProcDecl =
            std::dynamic_pointer_cast<ProcDeclNode>(lhs.declarations_.at(i));
        const auto rhsProcDecl =
            std::dynamic_pointer_cast<ProcDeclNode>(rhs.declarations_.at(i));

        if (*lhsProcDecl != *rhsProcDecl)
          return false;

        break;
      }
      case DeclNodeType::VAR_DECL: {
        const auto lhsVarDecl =
            std::dynamic_pointer_cast<VarDeclNode>(lhs.declarations_.at(i));
        const auto rhsVarDecl =
            std::dynamic_pointer_cast<VarDeclNode>(rhs.declarations_.at(i));

        if (*lhsVarDecl != *rhsVarDecl)
          return false;

        break;
      }
      default: {
        break;
      }
      }
    }

    for (std::size_t i = 0; i < lhs.statements_.size(); ++i)
    {
      if (lhs.statements_.at(i)->getType() != rhs.statements_.at(i)->getType())
        return false;

      switch (lhs.statements_.at(i)->getType())
      {
      case StmtNodeType::ASSIGN_STMT: {
        const auto lhsAssignStmt =
            std::dynamic_pointer_cast<AssignStmtNode>(lhs.statements_.at(i));
        const auto rhsAssignStmt =
            std::dynamic_pointer_cast<AssignStmtNode>(rhs.statements_.at(i));

        if (*lhsAssignStmt != *rhsAssignStmt)
          return false;

        break;
      }
      default: {
        break;
      }
      }
    }

    return true;
  }();

  return sameDeclarationList;
}

bool operator!=(const BlockNode &lhs, const BlockNode &rhs)
{
  return !(lhs == rhs);
}

}; // namespace parser
}; // namespace pl0c
