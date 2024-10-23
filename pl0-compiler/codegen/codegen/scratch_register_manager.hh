#pragma once

#include <map>
#include <string>

namespace pl0c
{
namespace codegen
{

enum class ScratchRegister
{
  NONE,
  RAX,
  R8,
  R9,
  R10,
  R11,
  R12,
  R13,
  R14,
  R15
};

class ScratchRegisterManager
{
public:
  ScratchRegisterManager() = default;

  auto alloc() -> ScratchRegister;
  auto free(const ScratchRegister reg) -> void;

private:
  std::map<ScratchRegister, bool> registerAllocs_ = {
      {ScratchRegister::RAX, false}, {ScratchRegister::R11, false},
      {ScratchRegister::R8, false},  {ScratchRegister::R12, false},
      {ScratchRegister::R9, false},  {ScratchRegister::R13, false},
      {ScratchRegister::R10, false}, {ScratchRegister::R14, false},
      {ScratchRegister::R15, false}};
};

const auto scratchRegisterStringMap = std::map<ScratchRegister, std::string>{
    {ScratchRegister::RAX, "%rax"}, {ScratchRegister::R11, "%r11"},
    {ScratchRegister::R8, "%r8"},   {ScratchRegister::R12, "%r12"},
    {ScratchRegister::R9, "%r9"},   {ScratchRegister::R13, "%r13"},
    {ScratchRegister::R10, "%r10"}, {ScratchRegister::R14, "%r14"},
    {ScratchRegister::R15, "%r15"}};

}; // namespace codegen
}; // namespace pl0c
