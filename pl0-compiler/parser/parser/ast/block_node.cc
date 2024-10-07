#include "parser/ast/block_node.hh"

namespace pl0c
{
namespace parser
{

BlockNode::BlockNode()
{
}

bool operator==([[maybe_unused]] const BlockNode &lhs,
                [[maybe_unused]] const BlockNode &rhs)
{
  return true;
}

bool operator!=([[maybe_unused]] const BlockNode &lhs,
                [[maybe_unused]] const BlockNode &rhs)
{
  return !(lhs == rhs);
}

}; // namespace parser
}; // namespace pl0c
