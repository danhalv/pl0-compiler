#pragma once

#include "lexer/token/token.hh"

#include <memory>
#include <vector>

namespace pl0c
{
namespace lexer
{

auto run(std::vector<unsigned char> text)
    -> std::vector<std::shared_ptr<Token>>;

}; // namespace lexer
}; // namespace pl0c
