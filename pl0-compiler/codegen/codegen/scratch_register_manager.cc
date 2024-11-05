#include "codegen/scratch_register_manager.hh"

namespace pl0c
{
namespace codegen
{

auto ScratchRegisterManager::alloc() -> ScratchRegister
{
  for (auto it = registerAllocs_.begin(); it != registerAllocs_.end(); ++it)
  {
    if (it->second == false)
    {
      it->second = true;
      return it->first;
    }
  }

  return ScratchRegister::NONE;
}

auto ScratchRegisterManager::free(const ScratchRegister reg) -> void
{
  auto it = registerAllocs_.find(reg);
  it->second = false;
}

}; // namespace codegen
}; // namespace pl0c
