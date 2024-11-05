#pragma once

#include "parser/ast/program_node.hh"

namespace pl0c
{
namespace codegen
{

auto run(const parser::ProgramNode &) -> void;

};
}; // namespace pl0c
