#include "lexer/lexer.hh"

namespace pl0c
{
namespace lexer
{

auto run(std::vector<unsigned char> text) -> std::vector<std::shared_ptr<Token>>
{
  auto tokens = std::vector<std::shared_ptr<Token>>{};

  for (unsigned int i = 0; i < text.size(); ++i)
  {
    switch (text.at(i))
    {
    case '.': {
      tokens.push_back(std::make_shared<Token>(TokenType::DOT));
      break;
    }
    default: {
      break;
    }
    }
  }

  return tokens;
}

}; // namespace lexer
}; // namespace pl0c
