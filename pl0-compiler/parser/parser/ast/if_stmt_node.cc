#include "parser/ast/if_stmt_node.hh"

#include "parser/ast/assign_stmt_node.hh"
#include "parser/ast/call_stmt_node.hh"
#include "parser/ast/odd_test_node.hh"

namespace pl0c
{
namespace parser
{

IfStmtNode::IfStmtNode(const std::shared_ptr<TestNode> testNode,
                       const std::vector<std::shared_ptr<StmtNode>> &statements)
    : testNode_{testNode}, statements_{statements}
{
}

auto IfStmtNode::getStatements() const -> std::vector<std::shared_ptr<StmtNode>>
{
  return this->statements_;
}

auto IfStmtNode::getTestNode() const -> std::shared_ptr<TestNode>
{
  return this->testNode_;
}

auto IfStmtNode::getType() const -> StmtNodeType
{
  return StmtNodeType::IF_STMT;
}

bool operator==(const IfStmtNode &lhs, const IfStmtNode &rhs)
{
  const bool sameTestNode = [&lhs, &rhs]() {
    if (lhs.testNode_->getType() != rhs.testNode_->getType())
      return false;

    switch (lhs.testNode_->getType())
    {
    case TestNodeType::ODD_TEST: {
      const auto lhsOddTest =
          std::dynamic_pointer_cast<OddTestNode>(lhs.testNode_);
      const auto rhsOddTest =
          std::dynamic_pointer_cast<OddTestNode>(rhs.testNode_);

      return (*lhsOddTest == *rhsOddTest);
    }
    default: {
      return false;
    }
    }
  }();

  const bool sameStatementList = [&lhs, &rhs]() {
    if (lhs.statements_.size() != rhs.statements_.size())
      return false;

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

        return (*lhsAssignStmt == *rhsAssignStmt);
      }
      case StmtNodeType::CALL_STMT: {
        const auto lhsCallStmt =
            std::dynamic_pointer_cast<CallStmtNode>(lhs.statements_.at(i));
        const auto rhsCallStmt =
            std::dynamic_pointer_cast<CallStmtNode>(rhs.statements_.at(i));

        return (*lhsCallStmt == *rhsCallStmt);
      }
      case StmtNodeType::IF_STMT: {
        const auto lhsIfStmt =
            std::dynamic_pointer_cast<IfStmtNode>(lhs.statements_.at(i));
        const auto rhsIfStmt =
            std::dynamic_pointer_cast<IfStmtNode>(rhs.statements_.at(i));

        return (*lhsIfStmt == *rhsIfStmt);
      }
      default: {
        return false;
      }
      }
    }

    return true;
  }();

  return (sameTestNode && sameStatementList);
}

bool operator!=(const IfStmtNode &lhs, const IfStmtNode &rhs)
{
  return !(lhs == rhs);
}

}; // namespace parser
}; // namespace pl0c
