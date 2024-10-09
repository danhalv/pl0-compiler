#pragma once

namespace pl0c
{
namespace parser
{

enum class DeclNodeType
{
  CONST_DECL,
};

class DeclNode
{
public:
  virtual ~DeclNode() = default;
  virtual auto getType() const -> DeclNodeType = 0;
};

}; // namespace parser
}; // namespace pl0c
