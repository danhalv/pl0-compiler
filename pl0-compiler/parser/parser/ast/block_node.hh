#pragma once

#include "parser/ast/decl_node.hh"
#include "parser/ast/stmt_node.hh"

#include <memory>
#include <vector>

namespace pl0c
{
namespace parser
{

class BlockNode
{
public:
  BlockNode() = delete;
  BlockNode(const std::vector<std::shared_ptr<DeclNode>> &,
            const std::vector<std::shared_ptr<StmtNode>> &);

  auto getDeclarations() const -> std::vector<std::shared_ptr<DeclNode>>;
  auto getStatements() const -> std::vector<std::shared_ptr<StmtNode>>;

  friend bool operator==(const BlockNode &, const BlockNode &);
  friend bool operator!=(const BlockNode &, const BlockNode &);

private:
  const std::vector<std::shared_ptr<DeclNode>> declarations_;
  const std::vector<std::shared_ptr<StmtNode>> statements_;
};

bool operator==(const BlockNode &, const BlockNode &);
bool operator!=(const BlockNode &, const BlockNode &);

}; // namespace parser
}; // namespace pl0c
