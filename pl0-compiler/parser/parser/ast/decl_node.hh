#pragma once

#include <string>

namespace pl0c
{
namespace parser
{

enum class DeclNodeType
{
  CONST_DECL,
  VAR_DECL,
};

class DeclNode
{
public:
  virtual ~DeclNode() = default;
  virtual auto getDeclId() const -> std::string = 0;
  virtual auto getType() const -> DeclNodeType = 0;
};

}; // namespace parser
}; // namespace pl0c
