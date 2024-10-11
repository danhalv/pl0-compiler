#pragma once

namespace pl0c
{
namespace parser
{

enum class ExprNodeType
{
  INT_EXPR,
  PLUS_EXPR,
};

class ExprNode
{
public:
  virtual ~ExprNode() = default;
  virtual auto getType() const -> ExprNodeType = 0;
};

}; // namespace parser
}; // namespace pl0c
