#pragma once

#include <cstdint>
#include <string>

namespace pl0c
{
namespace codegen
{
class LabelManager
{
public:
  LabelManager() = default;

  auto genLabel(const std::string &label) -> std::string;

private:
  std::uint64_t labelIndex = 0;
};

}; // namespace codegen
}; // namespace pl0c
