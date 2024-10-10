#pragma once

#include "lexer/token/token.hh"
#include "parser/ast/decl_node.hh"

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace pl0c
{
namespace parser
{

class BlockNode;
bool operator==(const BlockNode &, const BlockNode &);

class ProcDeclNode : public DeclNode
{
public:
  ProcDeclNode() = delete;
  ProcDeclNode(
      const std::string &declId,
      const std::vector<std::pair<std::string, std::shared_ptr<lexer::Token>>>
          arguments,
      const std::shared_ptr<BlockNode>);

  auto getArguments() const
      -> std::vector<std::pair<std::string, std::shared_ptr<lexer::Token>>>;
  auto getDeclBlockNode() const -> std::shared_ptr<BlockNode>;
  auto getDeclId() const -> std::string override;
  auto getType() const -> DeclNodeType override;

  friend bool operator==(const ProcDeclNode &, const ProcDeclNode &);
  friend bool operator!=(const ProcDeclNode &, const ProcDeclNode &);

private:
  const std::string declId_;
  const std::vector<std::pair<std::string, std::shared_ptr<lexer::Token>>>
      arguments_;
  const std::shared_ptr<BlockNode> blockNode_;
};

bool operator==(const ProcDeclNode &, const ProcDeclNode &);
bool operator!=(const ProcDeclNode &, const ProcDeclNode &);

}; // namespace parser
}; // namespace pl0c
