#include "lexer/lexer.hh"
#include "lexer/token/token.hh"

#include <gtest/gtest.h>
#include <vector>

TEST(LexerTest, lexEmptyText)
{
  const auto emptyText = std::vector<unsigned char>{};
  ASSERT_EQ(pl0c::lexer::run(emptyText).size(), 0);
}

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

TEST(LexerTest, lexPunctuations)
{
  const auto text = std::vector<unsigned char>{',', '.', ';'};
  const auto tokens = pl0c::lexer::run(text);

  EXPECT_EQ(*tokens.at(0), pl0c::lexer::Token{pl0c::lexer::TokenType::COMMA});
  EXPECT_EQ(*tokens.at(1), pl0c::lexer::Token{pl0c::lexer::TokenType::DOT});
  EXPECT_EQ(*tokens.at(2),
            pl0c::lexer::Token{pl0c::lexer::TokenType::SEMI_COLON});
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
