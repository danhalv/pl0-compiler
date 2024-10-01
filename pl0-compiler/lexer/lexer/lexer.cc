#include "lexer/lexer.hh"

#include "lexer/token/id_token.hh"
#include "lexer/token/integer_token.hh"
#include "lexer/token/token_type.hh"

#include <cctype>

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
    case ':': {
      tokens.push_back(std::make_shared<Token>(TokenType::COLON));
      break;
    }
    case ',': {
      tokens.push_back(std::make_shared<Token>(TokenType::COMMA));
      break;
    }
    case '.': {
      tokens.push_back(std::make_shared<Token>(TokenType::DOT));
      break;
    }
    case '(': {
      tokens.push_back(std::make_shared<Token>(TokenType::LEFT_PAREN));
      break;
    }
    case ')': {
      tokens.push_back(std::make_shared<Token>(TokenType::RIGHT_PAREN));
      break;
    }
    case ';': {
      tokens.push_back(std::make_shared<Token>(TokenType::SEMI_COLON));
      break;
    }
    default: {
      // Identifier token
      // ID           ::= Letter AlphaNumeric*
      // AlphaNumeric ::= Letter|Digit
      // Letter       ::= a|...|z|A|...|Z
      // Digit        ::= 0|...|9
      if (std::isalpha(text.at(i)))
      {
        auto identifier = std::string{text.at(i)};
        ++i;
        while (std::isalnum(text.at(i)))
        {
          identifier += std::string{text.at(i)};
          ++i;
        }

        tokens.push_back(std::make_shared<IdToken>(IdToken(identifier)));
      }

      // Integer token
      // Integer  ::= Digit+
      // Digit    ::= 0|...|9
      if (std::isdigit(static_cast<int>(text.at(i))))
      {
        auto integerString = std::string{text.at(i)};
        ++i;
        while (std::isdigit(static_cast<int>(text.at(i))))
        {
          integerString += std::string{text.at(i)};
          ++i;
        }

        tokens.push_back(
            std::make_shared<IntegerToken>(IntegerToken(integerString)));
      }

      break;
    }
    }
  }

  return tokens;
}

}; // namespace lexer
}; // namespace pl0c
