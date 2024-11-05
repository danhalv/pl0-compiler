#include "codegen/label_manager.hh"

namespace pl0c
{
namespace codegen
{

auto LabelManager::genLabel(const std::string &label) -> std::string
{
  return std::string{label + "_" + std::to_string(labelIndex++)};
}

}; // namespace codegen
}; // namespace pl0c
