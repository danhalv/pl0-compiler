#pragma once

namespace pl0c
{
namespace parser
{

enum class TestNodeType
{
  EQUAL_TEST,
  ODD_TEST,
};

class TestNode
{
public:
  virtual ~TestNode() = default;
  virtual auto getType() const -> TestNodeType = 0;
};

}; // namespace parser
}; // namespace pl0c
