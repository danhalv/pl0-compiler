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
      // := walrus operator
      if ((i + 1) < text.size() && '=' == text.at(i + 1))
      {
        tokens.push_back(std::make_shared<Token>(TokenType::WALRUS));
        ++i;
      }
      else
      {
        tokens.push_back(std::make_shared<Token>(TokenType::COLON));
      }
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
    case '*': {
      tokens.push_back(std::make_shared<Token>(TokenType::ASTERISK));
      break;
    }
    case '/': {
      tokens.push_back(std::make_shared<Token>(TokenType::SLASH));
      break;
    }
    case '+': {
      tokens.push_back(std::make_shared<Token>(TokenType::PLUS));
      break;
    }
    case '-': {
      tokens.push_back(std::make_shared<Token>(TokenType::MINUS));
      break;
    }
    case '=': {
      tokens.push_back(std::make_shared<Token>(TokenType::ASSIGNMENT));
      break;
    }
    case '<': {
      if ((i + 1) < text.size())
      {
        switch (text.at(i + 1))
        {
        // <> diamond operator
        case '>': {
          tokens.push_back(std::make_shared<Token>(TokenType::DIAMOND));
          ++i;
          break;
        }
        // <= less than or equal to operator
        case '=': {
          tokens.push_back(std::make_shared<Token>(TokenType::LEQ));
          ++i;
          break;
        }
        // < less than operator
        default: {
          tokens.push_back(std::make_shared<Token>(TokenType::LE));
          break;
        }
        }
      }
      else
      {
        tokens.push_back(std::make_shared<Token>(TokenType::LE));
      }
      break;
    }
    case '>': {
      // >= greater than or equal to operator
      if ((i + 1) < text.size() && '=' == text.at(i + 1))
      {
        tokens.push_back(std::make_shared<Token>(TokenType::GEQ));
        ++i;
      }
      // > greater than operator
      else
      {
        tokens.push_back(std::make_shared<Token>(TokenType::GE));
      }
      break;
    }
    default: {
      // Identifier token (also handles keywords)
      // ID           ::= Letter AlphaNumeric*
      // AlphaNumeric ::= Letter|Digit
      // Letter       ::= a|...|z|A|...|Z
      // Digit        ::= 0|...|9
      if (std::isalpha(text.at(i)))
      {
        auto identifier = std::string{static_cast<char>(text.at(i))};
        ++i;
        while (std::isalnum(text.at(i)))
        {
          identifier += std::string{static_cast<char>(text.at(i))};
          ++i;
        }

        if ("module" == identifier)
        {
          tokens.push_back(std::make_shared<Token>(TokenType::MODULE));
        }
        else if ("procedure" == identifier)
        {
          tokens.push_back(std::make_shared<Token>(TokenType::PROCEDURE));
        }
        else if ("begin" == identifier)
        {
          tokens.push_back(std::make_shared<Token>(TokenType::BEGIN));
        }
        else if ("end" == identifier)
        {
          tokens.push_back(std::make_shared<Token>(TokenType::END));
        }
        else if ("const" == identifier)
        {
          tokens.push_back(std::make_shared<Token>(TokenType::CONST));
        }
        else if ("var" == identifier)
        {
          tokens.push_back(std::make_shared<Token>(TokenType::VAR));
        }
        else if ("int" == identifier)
        {
          tokens.push_back(std::make_shared<Token>(TokenType::INT));
        }
        else if ("if" == identifier)
        {
          tokens.push_back(std::make_shared<Token>(TokenType::IF));
        }
        else if ("then" == identifier)
        {
          tokens.push_back(std::make_shared<Token>(TokenType::THEN));
        }
        else if ("while" == identifier)
        {
          tokens.push_back(std::make_shared<Token>(TokenType::WHILE));
        }
        else if ("do" == identifier)
        {
          tokens.push_back(std::make_shared<Token>(TokenType::DO));
        }
        else if ("input" == identifier)
        {
          tokens.push_back(std::make_shared<Token>(TokenType::INPUT));
        }
        else if ("output" == identifier)
        {
          tokens.push_back(std::make_shared<Token>(TokenType::OUTPUT));
        }
        else if ("odd" == identifier)
        {
          tokens.push_back(std::make_shared<Token>(TokenType::ODD));
        }
        else
        {
          tokens.push_back(std::make_shared<IdToken>(IdToken(identifier)));
        }
      }

      // Integer token
      // Integer  ::= Digit+
      // Digit    ::= 0|...|9
      if (std::isdigit(static_cast<int>(text.at(i))))
      {
        auto integerString = std::string{static_cast<char>(text.at(i))};
        ++i;
        while (std::isdigit(static_cast<int>(text.at(i))))
        {
          integerString += std::string{static_cast<char>(text.at(i))};
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
