#include "parser/ast/program_node.hh"

#include "parser/ast/block_node.hh"

namespace pl0c
{
namespace parser
{

ProgramNode::ProgramNode(const std::string id, const BlockNode &blockNode)
    : id_{id}, blockNode_{blockNode}
{
}

auto ProgramNode::getId() const -> std::string
{
  return id_;
}

auto ProgramNode::getBlockNode() const -> const BlockNode &
{
  return blockNode_;
}

bool operator==(const ProgramNode &lhs, const ProgramNode &rhs)
{
  const bool sameId = (lhs.getId() == rhs.getId());
  const bool sameBlockNode = (lhs.getBlockNode() == rhs.getBlockNode());

  return (sameId && sameBlockNode);
}

bool operator!=(const ProgramNode &lhs, const ProgramNode &rhs)
{
  return !(lhs == rhs);
}

}; // namespace parser
}; // namespace pl0c
