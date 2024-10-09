#include "parser/ast/block_node.hh"

namespace pl0c
{
namespace parser
{

BlockNode::BlockNode(const std::vector<std::shared_ptr<DeclNode>> &declarations)
    : declarations_{declarations}
{
}

auto BlockNode::getDeclarations() const
    -> std::vector<std::shared_ptr<DeclNode>>
{
  return this->declarations_;
}

bool operator==(const BlockNode &lhs, const BlockNode &rhs)
{
  const bool sameDeclarationList = (lhs.declarations_ == rhs.declarations_);

  return sameDeclarationList;
}

bool operator!=(const BlockNode &lhs, const BlockNode &rhs)
{
  return !(lhs == rhs);
}

}; // namespace parser
}; // namespace pl0c
