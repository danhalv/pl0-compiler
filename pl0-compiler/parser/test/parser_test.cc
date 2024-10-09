#include "lexer/lexer.hh"
#include "lexer/token/id_token.hh"
#include "lexer/token/integer_token.hh"
#include "parser/ast/block_node.hh"
#include "parser/ast/const_decl_node.hh"
#include "parser/ast/program_node.hh"
#include "parser/parser.hh"

#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <vector>

auto createText(const std::string &textString) -> std::vector<unsigned char>
{
  auto text = std::vector<unsigned char>(textString.length());
  text.insert(text.begin(), textString.begin(), textString.end());

  return text;
}

TEST(ParserBlockConstDeclTest, constDeclarationIntLiteral)
{
  const auto textString =
      std::string{"module myModule; const i : int = 1; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));
  const auto programNode = pl0c::parser::run(tokens);

  const auto &actualDeclNode =
      *std::dynamic_pointer_cast<pl0c::parser::ConstDeclNode>(
          programNode.getBlockNode().getDeclarations().front());
  const auto expectedDeclNode = pl0c::parser::ConstDeclNode{
      "i", std::make_shared<pl0c::lexer::Token>(pl0c::lexer::TokenType::INT),
      std::make_shared<pl0c::lexer::IntegerToken>("1")};

  ASSERT_EQ(actualDeclNode, expectedDeclNode);
}

TEST(ParserBlockConstDeclTest, constDeclarationIntIdentifier)
{
  const auto textString =
      std::string{"module myModule; const i : int = x; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));
  const auto programNode = pl0c::parser::run(tokens);

  const auto &actualDeclNode =
      *std::dynamic_pointer_cast<pl0c::parser::ConstDeclNode>(
          programNode.getBlockNode().getDeclarations().front());
  const auto expectedDeclNode = pl0c::parser::ConstDeclNode{
      "i", std::make_shared<pl0c::lexer::Token>(pl0c::lexer::TokenType::INT),
      std::make_shared<pl0c::lexer::IdToken>("x")};

  ASSERT_EQ(actualDeclNode, expectedDeclNode);
}

TEST(ParserBlockConstDeclTest, multipleConstDeclarations)
{
  const auto textString = std::string{
      "module myModule; const i : int = 1, x : int = i; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));
  const auto programNode = pl0c::parser::run(tokens);

  const auto actualDeclNodes = programNode.getBlockNode().getDeclarations();
  const auto expectedDeclNodes =
      std::vector<std::shared_ptr<pl0c::parser::ConstDeclNode>>{
          std::make_shared<pl0c::parser::ConstDeclNode>(
              "i",
              std::make_shared<pl0c::lexer::Token>(pl0c::lexer::TokenType::INT),
              std::make_shared<pl0c::lexer::IntegerToken>("1")),
          std::make_shared<pl0c::parser::ConstDeclNode>(
              "x",
              std::make_shared<pl0c::lexer::Token>(pl0c::lexer::TokenType::INT),
              std::make_shared<pl0c::lexer::IdToken>("i"))};

  ASSERT_EQ(*std::dynamic_pointer_cast<pl0c::parser::ConstDeclNode>(
                actualDeclNodes.at(0)),
            *expectedDeclNodes.at(0));
  ASSERT_EQ(*std::dynamic_pointer_cast<pl0c::parser::ConstDeclNode>(
                actualDeclNodes.at(1)),
            *expectedDeclNodes.at(1));
}

TEST(ParserBlockConstDeclTest, constDeclarationMissingConstKeyword)
{
  const auto textString =
      std::string{"module myModule; i : int = x; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}

TEST(ParserBlockConstDeclTest, constDeclarationMissingColon)
{
  const auto textString =
      std::string{"module myModule; const i int = x; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}

TEST(ParserBlockConstDeclTest, constDeclarationMissingType)
{
  const auto textString =
      std::string{"module myModule; const i : = x; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}

TEST(ParserBlockConstDeclTest, constDeclarationInvalidType)
{
  const auto textString =
      std::string{"module myModule; const i : i = x; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}

TEST(ParserBlockConstDeclTest, constDeclarationMissingAssignmentOperator)
{
  const auto textString =
      std::string{"module myModule; const i : int x; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}

TEST(ParserBlockConstDeclTest, constDeclarationMissingExpr)
{
  const auto textString =
      std::string{"module myModule; const i : int = ; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}

TEST(ParserBlockConstDeclTest, constDeclarationInvalidExpr)
{
  const auto textString = std::string{
      "module myModule; const i : int = const; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}

TEST(ParserProgramSymbolTest, programSymbol)
{
  const auto textString = std::string{"module myModule; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));
  const auto programNode = pl0c::parser::run(tokens);

  ASSERT_EQ(programNode,
            pl0c::parser::ProgramNode("myModule", pl0c::parser::BlockNode{{}}));
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
