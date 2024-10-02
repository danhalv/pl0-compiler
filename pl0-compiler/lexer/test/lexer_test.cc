#include "lexer/lexer.hh"
#include "lexer/token/id_token.hh"
#include "lexer/token/integer_token.hh"
#include "lexer/token/token.hh"

#include <cstdint>
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

// ===KEYWORD TESTS===

TEST(LexerTest, lexModuleToken)
{
  const auto moduleKeyword = std::string{"module"};
  auto text = std::vector<unsigned char>(moduleKeyword.length());
  text.insert(text.begin(), moduleKeyword.begin(), moduleKeyword.end());

  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::MODULE});
}

TEST(LexerTest, lexProcedureToken)
{
  const auto procedureKeyword = std::string{"procedure"};
  auto text = std::vector<unsigned char>(procedureKeyword.length());
  text.insert(text.begin(), procedureKeyword.begin(), procedureKeyword.end());

  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::PROCEDURE});
}

TEST(LexerTest, lexBeginToken)
{
  const auto beginKeyword = std::string{"begin"};
  auto text = std::vector<unsigned char>(beginKeyword.length());
  text.insert(text.begin(), beginKeyword.begin(), beginKeyword.end());

  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::BEGIN});
}

TEST(LexerTest, lexEndToken)
{
  const auto endKeyword = std::string{"end"};
  auto text = std::vector<unsigned char>(endKeyword.length());
  text.insert(text.begin(), endKeyword.begin(), endKeyword.end());

  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::END});
}

TEST(LexerTest, lexConstToken)
{
  const auto constKeyword = std::string{"const"};
  auto text = std::vector<unsigned char>(constKeyword.length());
  text.insert(text.begin(), constKeyword.begin(), constKeyword.end());

  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::CONST});
}

TEST(LexerTest, lexVarToken)
{
  const auto varKeyword = std::string{"var"};
  auto text = std::vector<unsigned char>(varKeyword.length());
  text.insert(text.begin(), varKeyword.begin(), varKeyword.end());

  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::VAR});
}

TEST(LexerTest, lexIntToken)
{
  const auto intKeyword = std::string{"int"};
  auto text = std::vector<unsigned char>(intKeyword.length());
  text.insert(text.begin(), intKeyword.begin(), intKeyword.end());

  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::INT});
}

TEST(LexerTest, lexIfToken)
{
  const auto ifKeyword = std::string{"if"};
  auto text = std::vector<unsigned char>(ifKeyword.length());
  text.insert(text.begin(), ifKeyword.begin(), ifKeyword.end());

  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::IF});
}

TEST(LexerTest, lexThenToken)
{
  const auto thenKeyword = std::string{"then"};
  auto text = std::vector<unsigned char>(thenKeyword.length());
  text.insert(text.begin(), thenKeyword.begin(), thenKeyword.end());

  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::THEN});
}

TEST(LexerTest, lexWhileToken)
{
  const auto whileKeyword = std::string{"while"};
  auto text = std::vector<unsigned char>(whileKeyword.length());
  text.insert(text.begin(), whileKeyword.begin(), whileKeyword.end());

  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::WHILE});
}

TEST(LexerTest, lexDoToken)
{
  const auto doKeyword = std::string{"do"};
  auto text = std::vector<unsigned char>(doKeyword.length());
  text.insert(text.begin(), doKeyword.begin(), doKeyword.end());

  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::DO});
}

TEST(LexerTest, lexInputToken)
{
  const auto inputKeyword = std::string{"input"};
  auto text = std::vector<unsigned char>(inputKeyword.length());
  text.insert(text.begin(), inputKeyword.begin(), inputKeyword.end());

  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::INPUT});
}

TEST(LexerTest, lexOutputToken)
{
  const auto outputKeyword = std::string{"output"};
  auto text = std::vector<unsigned char>(outputKeyword.length());
  text.insert(text.begin(), outputKeyword.begin(), outputKeyword.end());

  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::OUTPUT});
}

TEST(LexerTest, lexOddToken)
{
  const auto oddKeyword = std::string{"odd"};
  auto text = std::vector<unsigned char>(oddKeyword.length());
  text.insert(text.begin(), oddKeyword.begin(), oddKeyword.end());

  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::ODD});
}

// ===END OF KEYWORD TESTS===

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

// ===WHITESPACE TESTS===

TEST(LexerTest, lexWhitespaceTokenSpace)
{
  const auto text = std::vector<unsigned char>{' '};
  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::WHITESPACE});
}

TEST(LexerTest, lexWhitespaceTokenTab)
{
  const auto text = std::vector<unsigned char>{'\t'};
  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::WHITESPACE});
}

TEST(LexerTest, lexWhitespaceTokenNewLine)
{
  const auto text = std::vector<unsigned char>{'\n'};
  ASSERT_EQ(*pl0c::lexer::run(text).front(),
            pl0c::lexer::Token{pl0c::lexer::TokenType::WHITESPACE});
}

// ===END OF WHITESPACE TESTS===

// ===OTHER TESTS===

TEST(LexerTest, lexEmptyText)
{
  const auto emptyText = std::vector<unsigned char>{};
  ASSERT_EQ(pl0c::lexer::run(emptyText).size(), 0);
}

TEST(LexerTest, lexTextWithDifferentTokens)
{
  const auto textString = std::string{"procedure proc;\nvar i;\nbegin\nend;"};
  auto text = std::vector<unsigned char>(textString.length());
  text.insert(text.begin(), textString.begin(), textString.end());

  const auto actualTokens = pl0c::lexer::run(text);
  const auto actualTokenString = pl0c::lexer::toString(actualTokens);

  const auto expectedTokenString =
      std::string("Token(PROCEDURE)Token(WHITESPACE)IdToken(\"proc\")Token("
                  "SEMI_COLON)Token(WHITESPACE)Token(VAR)Token(WHITESPACE)"
                  "IdToken(\"i\")Token(SEMI_COLON)Token(WHITESPACE)Token(BEGIN)"
                  "Token(WHITESPACE)Token(END)Token(SEMI_COLON)");

  ASSERT_TRUE(actualTokenString == expectedTokenString);
}

// ===END OF OTHER TESTS===
