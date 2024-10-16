#pragma once

namespace pl0c
{
namespace parser
{

enum class StmtNodeType
{
  ASSIGN_STMT,
  CALL_STMT,
  IF_STMT,
  OUT_STMT,
};

class StmtNode
{
public:
  virtual ~StmtNode() = default;
  virtual auto getType() const -> StmtNodeType = 0;
};

}; // namespace parser
}; // namespace pl0c
