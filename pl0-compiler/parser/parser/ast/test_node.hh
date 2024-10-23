#pragma once

namespace pl0c
{
namespace parser
{

enum class TestNodeType
{
  EQ_TEST,
  GE_TEST,
  GEQ_TEST,
  LE_TEST,
  LEQ_TEST,
  NEQ_TEST,
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
