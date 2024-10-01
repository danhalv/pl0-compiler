#include "lexer/lexer.hh"
#include "lexer/token/id_token.hh"
#include "lexer/token/integer_token.hh"
#include "lexer/token/token.hh"

#include <gtest/gtest.h>
#include <memory>
#include <vector>

// ===LITERAL TESTS===

TEST(LexerTest, lexIdToken)
{
  const auto identifier = std::string{"anIdToken123"};
  auto text = std::vector<unsigned char>(identifier.length());
  text.insert(text.begin(), identifier.begin(), identifier.end());

  const auto result = pl0c::lexer::run(text);
  auto resultToken =
      *std::dynamic_pointer_cast<pl0c::lexer::IdToken>(result.front());
  const auto expectedToken = pl0c::lexer::IdToken(identifier);

  ASSERT_EQ(resultToken, expectedToken);
}

TEST(LexerTest, lexIntegerToken)
{
  const auto integerString = std::string{"09"};
  auto text = std::vector<unsigned char>(integerString.length());
  text.insert(text.begin(), integerString.begin(), integerString.end());

  const auto result = pl0c::lexer::run(text);
  auto resultToken =
      *std::dynamic_pointer_cast<pl0c::lexer::IntegerToken>(result.front());
  const auto expectedToken = pl0c::lexer::IntegerToken(integerString);

  EXPECT_EQ(resultToken, expectedToken);
  EXPECT_EQ(resultToken.getInt(), expectedToken.getInt());
}

// ===END OF LITERAL TESTS===

// ===PUNCTUATION TESTS===

TEST(LexerTest, lexColonToken)
{
  const auto text = std::vector<unsigned char>{':'};
  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::COLON});
}

TEST(LexerTest, lexCommaToken)
{
  const auto text = std::vector<unsigned char>{','};
  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::COMMA});
}

TEST(LexerTest, lexDotToken)
{
  const auto text = std::vector<unsigned char>{'.'};
  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::DOT});
}

TEST(LexerTest, lexLeftParenToken)
{
  const auto text = std::vector<unsigned char>{'('};
  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::LEFT_PAREN});
}

TEST(LexerTest, lexRightParenToken)
{
  const auto text = std::vector<unsigned char>{')'};
  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::RIGHT_PAREN});
}

TEST(LexerTest, lexSemiColonToken)
{
  const auto text = std::vector<unsigned char>{';'};
  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::SEMI_COLON});
}

TEST(LexerTest, lexPunctuations)
{
  const auto text = std::vector<unsigned char>{',', '.', ';'};
  const auto tokens = pl0c::lexer::run(text);

  EXPECT_EQ(*tokens.at(0), pl0c::lexer::Token{pl0c::lexer::TokenType::COMMA});
  EXPECT_EQ(*tokens.at(1), pl0c::lexer::Token{pl0c::lexer::TokenType::DOT});
  EXPECT_EQ(*tokens.at(2),
            pl0c::lexer::Token{pl0c::lexer::TokenType::SEMI_COLON});
}

// ===END OF PUNCTUATION TESTS===

// ===OPERATOR TESTS===

TEST(LexerTest, lexWalrusToken)
{
  const auto text = std::vector<unsigned char>{':', '='};
  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::WALRUS});
}

TEST(LexerTest, lexAsteriskToken)
{
  const auto text = std::vector<unsigned char>{'*'};
  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::ASTERISK});
}

TEST(LexerTest, lexSlashToken)
{
  const auto text = std::vector<unsigned char>{'/'};
  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::SLASH});
}

TEST(LexerTest, lexPlusToken)
{
  const auto text = std::vector<unsigned char>{'+'};
  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::PLUS});
}

TEST(LexerTest, lexMinusToken)
{
  const auto text = std::vector<unsigned char>{'-'};
  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::MINUS});
}

TEST(LexerTest, lexAssigmentToken)
{
  const auto text = std::vector<unsigned char>{'='};
  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::ASSIGNMENT});
}

TEST(LexerTest, lexDiamondToken)
{
  const auto text = std::vector<unsigned char>{'<', '>'};
  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::DIAMOND});
}

TEST(LexerTest, lexLeqToken)
{
  const auto text = std::vector<unsigned char>{'<', '='};
  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::LEQ});
}

TEST(LexerTest, lexLeToken)
{
  const auto text = std::vector<unsigned char>{'<'};
  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::LE});
}

TEST(LexerTest, lexGeqToken)
{
  const auto text = std::vector<unsigned char>{'>', '='};
  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::GEQ});
}

TEST(LexerTest, lexGeToken)
{
  const auto text = std::vector<unsigned char>{'>'};
  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::GE});
}

// ===END OF OPERATOR TESTS===

// ===OTHER TESTS===

TEST(LexerTest, lexEmptyText)
{
  const auto emptyText = std::vector<unsigned char>{};
  ASSERT_EQ(pl0c::lexer::run(emptyText).size(), 0);
}

// ===END OF OTHER TESTS===
