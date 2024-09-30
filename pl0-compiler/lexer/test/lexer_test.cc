#include "lexer/lexer.hh"
#include "lexer/token/token.hh"

#include <gtest/gtest.h>
#include <vector>

TEST(LexerTest, lexEmptyText)
{
  const auto emptyText = std::vector<unsigned char>{};
  ASSERT_EQ(pl0c::lexer::run(emptyText).size(), 0);
}

TEST(LexerTest, lexDotToken)
{
  const auto text = std::vector<unsigned char>{'.'};
  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::DOT});
}
