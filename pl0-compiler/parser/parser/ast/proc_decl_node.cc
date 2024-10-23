#include "parser/ast/proc_decl_node.hh"

#include <cstdint>

namespace pl0c
{
namespace parser
{

ProcDeclNode::ProcDeclNode(
    const std::string &declId,
    const std::vector<std::pair<std::string, std::shared_ptr<lexer::Token>>>
        arguments,
    const std::shared_ptr<BlockNode> blockNode)
    : declId_{declId}, arguments_{arguments}, blockNode_{blockNode}
{
}

auto ProcDeclNode::getArguments() const
    -> std::vector<std::pair<std::string, std::shared_ptr<lexer::Token>>>
{
  return this->arguments_;
}

auto ProcDeclNode::getDeclBlockNode() const -> std::shared_ptr<BlockNode>
{
  return this->blockNode_;
}

auto ProcDeclNode::getDeclId() const -> std::string
{
  return this->declId_;
}

auto ProcDeclNode::getType() const -> DeclNodeType
{
  return DeclNodeType::PROC_DECL;
}

bool operator==(const ProcDeclNode &lhs, const ProcDeclNode &rhs)
{
  const bool sameDeclId = (lhs.declId_ == rhs.declId_);
  const bool sameArguments = [&lhs, &rhs]() {
    if (lhs.arguments_.size() != rhs.arguments_.size())
      return false;

    for (std::size_t i = 0; i < lhs.arguments_.size(); ++i)
    {
      if (lhs.arguments_.at(i).first != rhs.arguments_.at(i).first)
        return false;

      if (*lhs.arguments_.at(i).second != *rhs.arguments_.at(i).second)
        return false;
    }

    return true;
  }();
  const bool sameBlockNode = (*lhs.blockNode_ == *rhs.blockNode_);

  return (sameDeclId && sameArguments && sameBlockNode);
}

bool operator!=(const ProcDeclNode &lhs, const ProcDeclNode &rhs)
{
  return !(lhs == rhs);
}

}; // namespace parser
}; // namespace pl0c
