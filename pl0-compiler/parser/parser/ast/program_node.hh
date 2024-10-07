#pragma once

#include "parser/ast/block_node.hh"

#include <string>

namespace pl0c
{
namespace parser
{

class ProgramNode
{
public:
  ProgramNode() = delete;
  ProgramNode(const std::string id, const BlockNode &);

  auto getId() const -> std::string;
  auto getBlockNode() const -> const BlockNode &;

  friend bool operator==(const ProgramNode &, const ProgramNode &);
  friend bool operator!=(const ProgramNode &, const ProgramNode &);

private:
  const std::string id_;
  const BlockNode blockNode_;
};

bool operator==(const ProgramNode &, const ProgramNode &);
bool operator!=(const ProgramNode &, const ProgramNode &);

}; // namespace parser
}; // namespace pl0c
