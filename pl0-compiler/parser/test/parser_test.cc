#include "lexer/lexer.hh"
#include "parser/ast/program_node.hh"
#include "parser/parser.hh"

#include <gtest/gtest.h>
#include <string>
#include <vector>

auto createText(const std::string &textString) -> std::vector<unsigned char>
{
  auto text = std::vector<unsigned char>(textString.length());
  text.insert(text.begin(), textString.begin(), textString.end());

  return text;
}

TEST(ParserProgramSymbolTest, programSymbol)
{
  const auto textString = std::string{"module myModule; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));
  const auto programNode = pl0c::parser::run(tokens);

  ASSERT_EQ(programNode,
            pl0c::parser::ProgramNode("myModule", pl0c::parser::BlockNode()));
}

TEST(ParserProgramSymbolTest, unequalIdentifiers)
{
  const auto textString =
      std::string{"module myModule; begin end wrongIdentifier."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}

TEST(ParserProgramSymbolTest, missingModuleToken)
{
  const auto textString = std::string{"myModule; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}

TEST(ParserProgramSymbolTest, missingInitialIdentifierToken)
{
  const auto textString = std::string{"module ; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}

TEST(ParserProgramSymbolTest, missingFinalIdentifierToken)
{
  const auto textString = std::string{"module myModule; begin end ."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}

TEST(ParserProgramSymbolTest, missingBlockStatement)
{
  const auto textString = std::string{"module ; end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}

TEST(ParserProgramSymbolTest, missingBlockEndToken)
{
  const auto textString = std::string{"module myModule; begin myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}

TEST(ParserProgramSymbolTest, missingDotToken)
{
  const auto textString = std::string{"module myModule; begin end myModule"};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}
