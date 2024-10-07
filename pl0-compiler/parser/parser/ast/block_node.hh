#pragma once

namespace pl0c
{
namespace parser
{

class BlockNode
{
public:
  BlockNode();

  friend bool operator==(const BlockNode &, const BlockNode &);
  friend bool operator!=(const BlockNode &, const BlockNode &);
};

bool operator==(const BlockNode &, const BlockNode &);
bool operator!=(const BlockNode &, const BlockNode &);

}; // namespace parser
}; // namespace pl0c
