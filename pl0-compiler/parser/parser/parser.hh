#pragma once

#include "lexer/token/token.hh"
#include "parser/ast/program_node.hh"

#include <memory>
#include <vector>

namespace pl0c
{
namespace parser
{

auto run(const std::vector<std::shared_ptr<lexer::Token>> &tokens)
    -> ProgramNode;

};
}; // namespace pl0c
