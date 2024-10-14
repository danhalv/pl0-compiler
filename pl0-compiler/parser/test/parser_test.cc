#include "lexer/lexer.hh"
#include "lexer/token/id_token.hh"
#include "lexer/token/integer_token.hh"
#include "parser/ast/assign_stmt_node.hh"
#include "parser/ast/block_node.hh"
#include "parser/ast/const_decl_node.hh"
#include "parser/ast/id_expr_node.hh"
#include "parser/ast/input_expr_node.hh"
#include "parser/ast/int_expr_node.hh"
#include "parser/ast/negative_expr_node.hh"
#include "parser/ast/plus_expr_node.hh"
#include "parser/ast/proc_decl_node.hh"
#include "parser/ast/program_node.hh"
#include "parser/ast/var_decl_node.hh"
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

TEST(ParserAssignStmtTest, assignStmt)
{
  const auto textString =
      std::string{"module myModule; begin i := 1; end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));
  const auto programNode = pl0c::parser::run(tokens);

  const auto actualStmtNode =
      *std::dynamic_pointer_cast<pl0c::parser::AssignStmtNode>(
          programNode.getBlockNode().getStatements().front());
  const auto expectedStmtNode = pl0c::parser::AssignStmtNode{
      "i", std::make_shared<pl0c::parser::IntExprNode>(1)};

  ASSERT_EQ(actualStmtNode, expectedStmtNode);
}

TEST(ParserAssignStmtTest, assignStmtMissingWalrusOperator)
{
  const auto textString =
      std::string{"module myModule; begin i 1; end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
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

TEST(ParserBlockDeclListTest, constAndVarDeclarations)
{
  const auto textString = std::string{
      "module myModule; const i : int = 1; var x : int; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));
  const auto programNode = pl0c::parser::run(tokens);

  const auto actualDeclNodes = programNode.getBlockNode().getDeclarations();
  const auto expectedDeclNodes =
      std::vector<std::shared_ptr<pl0c::parser::DeclNode>>{
          std::make_shared<pl0c::parser::ConstDeclNode>(
              "i",
              std::make_shared<pl0c::lexer::Token>(pl0c::lexer::TokenType::INT),
              std::make_shared<pl0c::lexer::IntegerToken>("1")),
          std::make_shared<pl0c::parser::VarDeclNode>(
              "x", std::make_shared<pl0c::lexer::Token>(
                       pl0c::lexer::TokenType::INT))};

  ASSERT_EQ(*std::dynamic_pointer_cast<pl0c::parser::ConstDeclNode>(
                actualDeclNodes.at(0)),
            *std::dynamic_pointer_cast<pl0c::parser::ConstDeclNode>(
                expectedDeclNodes.at(0)));
  ASSERT_EQ(*std::dynamic_pointer_cast<pl0c::parser::VarDeclNode>(
                actualDeclNodes.at(1)),
            *std::dynamic_pointer_cast<pl0c::parser::VarDeclNode>(
                expectedDeclNodes.at(1)));
}

TEST(ParserBlockProcDeclTest, procDeclaration)
{
  const auto textString =
      std::string{"module myModule; procedure myProcedure (); begin end "
                  "myProcedure; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));
  const auto programNode = pl0c::parser::run(tokens);

  const auto actualProcDeclNode =
      *std::dynamic_pointer_cast<pl0c::parser::ProcDeclNode>(
          programNode.getBlockNode().getDeclarations().at(0));
  const auto expectedProcDeclNode = pl0c::parser::ProcDeclNode{
      "myProcedure",
      {},
      std::make_shared<pl0c::parser::BlockNode>(
          std::vector<std::shared_ptr<pl0c::parser::DeclNode>>{},
          std::vector<std::shared_ptr<pl0c::parser::StmtNode>>{})};

  ASSERT_EQ(actualProcDeclNode, expectedProcDeclNode);
}

TEST(ParserBlockProcDeclTest, procDeclarationWithArguments)
{
  const auto textString = std::string{
      "module myModule; procedure myProcedure (i : int, x : int); begin end "
      "myProcedure; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));
  const auto programNode = pl0c::parser::run(tokens);

  const auto actualProcDeclNode =
      *std::dynamic_pointer_cast<pl0c::parser::ProcDeclNode>(
          programNode.getBlockNode().getDeclarations().at(0));
  const auto expectedProcDeclNode = pl0c::parser::ProcDeclNode{
      "myProcedure",
      {std::make_pair("i", std::make_shared<pl0c::lexer::Token>(
                               pl0c::lexer::TokenType::INT)),
       std::make_pair("x", std::make_shared<pl0c::lexer::Token>(
                               pl0c::lexer::TokenType::INT))},
      std::make_shared<pl0c::parser::BlockNode>(
          std::vector<std::shared_ptr<pl0c::parser::DeclNode>>{},
          std::vector<std::shared_ptr<pl0c::parser::StmtNode>>{})};

  ASSERT_EQ(actualProcDeclNode, expectedProcDeclNode);
}

TEST(ParserBlockProcDeclTest, procDeclarationMissingProcedureKeyword)
{
  const auto textString =
      std::string{"module myModule; myProcedure (); begin end "
                  "myProcedure; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}

TEST(ParserBlockProcDeclTest, procDeclarationMissingId)
{
  const auto textString =
      std::string{"module myModule; procedure (); begin end "
                  "myProcedure; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}

TEST(ParserBlockProcDeclTest, procDeclarationMissingEndId)
{
  const auto textString =
      std::string{"module myModule; procedure myProcedure(); begin end "
                  "; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}

TEST(ParserBlockProcDeclTest, procDeclarationMissingLeftParen)
{
  const auto textString =
      std::string{"module myModule; procedure myProcedure); begin end "
                  "myProcedure; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}

TEST(ParserBlockProcDeclTest, procDeclarationMissingRightParen)
{
  const auto textString =
      std::string{"module myModule; procedure myProcedure(; begin end "
                  "myProcedure; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}

TEST(ParserBlockProcDeclTest, procDeclarationMissingSemiColonAfterArguments)
{
  const auto textString =
      std::string{"module myModule; procedure myProcedure() begin end "
                  "myProcedure; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}

TEST(ParserBlockProcDeclTest, procDeclarationMissingEndSemiColon)
{
  const auto textString =
      std::string{"module myModule; procedure myProcedure(); begin end "
                  "myProcedure begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}

TEST(ParserBlockProcDeclTest, procDeclarationMissingBlock)
{
  const auto textString =
      std::string{"module myModule; procedure myProcedure(); myProcedure; "
                  "begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}

TEST(ParserBlockVarDeclTest, varDeclaration)
{
  const auto textString =
      std::string{"module myModule; var i : int; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));
  const auto programNode = pl0c::parser::run(tokens);

  const auto &actualDeclNode =
      *std::dynamic_pointer_cast<pl0c::parser::VarDeclNode>(
          programNode.getBlockNode().getDeclarations().front());
  const auto expectedDeclNode = pl0c::parser::VarDeclNode{
      "i", std::make_shared<pl0c::lexer::Token>(pl0c::lexer::TokenType::INT)};

  ASSERT_EQ(actualDeclNode, expectedDeclNode);
}

TEST(ParserBlockVarDeclTest, multipleVarDeclarations)
{
  const auto textString =
      std::string{"module myModule; var i : int, x : int; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));
  const auto programNode = pl0c::parser::run(tokens);

  const auto actualDeclNodes = programNode.getBlockNode().getDeclarations();
  const auto expectedDeclNodes =
      std::vector<std::shared_ptr<pl0c::parser::VarDeclNode>>{
          std::make_shared<pl0c::parser::VarDeclNode>(
              "i", std::make_shared<pl0c::lexer::Token>(
                       pl0c::lexer::TokenType::INT)),
          std::make_shared<pl0c::parser::VarDeclNode>(
              "x", std::make_shared<pl0c::lexer::Token>(
                       pl0c::lexer::TokenType::INT))};

  ASSERT_EQ(*std::dynamic_pointer_cast<pl0c::parser::VarDeclNode>(
                actualDeclNodes.at(0)),
            *expectedDeclNodes.at(0));
  ASSERT_EQ(*std::dynamic_pointer_cast<pl0c::parser::VarDeclNode>(
                actualDeclNodes.at(1)),
            *expectedDeclNodes.at(1));
}

TEST(ParserBlockVarDeclTest, varDeclarationMissingVarKeyword)
{
  const auto textString =
      std::string{"module myModule; i : int; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}

TEST(ParserBlockVarDeclTest, varDeclarationMissingIdentifier)
{
  const auto textString =
      std::string{"module myModule; var : int; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}

TEST(ParserBlockVarDeclTest, varDeclarationInvalidIdentifier)
{
  const auto textString =
      std::string{"module myModule; var const : int; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}

TEST(ParserBlockVarDeclTest, varDeclarationMissingColon)
{
  const auto textString =
      std::string{"module myModule; var i int; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}

TEST(ParserBlockVarDeclTest, varDeclarationMissingType)
{
  const auto textString =
      std::string{"module myModule; var i : ; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}

TEST(ParserBlockVarDeclTest, varDeclarationInvalidType)
{
  const auto textString =
      std::string{"module myModule; var i : x; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}

TEST(ParserExprTest, invalidExpr)
{
  const auto textString =
      std::string{"module myModule; begin i := const + 2; end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));

  ASSERT_DEATH(pl0c::parser::run(tokens), "");
}

TEST(ParserPlusExprTest, plusExprTwoIntegers)
{
  const auto textString =
      std::string{"module myModule; begin i := 1 + 2; end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));
  const auto programNode = pl0c::parser::run(tokens);

  const auto actualPlusExprNode =
      *std::dynamic_pointer_cast<pl0c::parser::PlusExprNode>(
          std::dynamic_pointer_cast<pl0c::parser::AssignStmtNode>(
              programNode.getBlockNode().getStatements().front())
              ->getExprNode());
  const auto expectedPlusExprNode = pl0c::parser::PlusExprNode{
      std::make_shared<pl0c::parser::IntExprNode>(1),
      std::make_shared<pl0c::parser::IntExprNode>(2)};

  ASSERT_EQ(actualPlusExprNode, expectedPlusExprNode);
}

TEST(ParserPlusExprTest, plusExprIntegerPlusId)
{
  const auto textString =
      std::string{"module myModule; begin i := 1 + x; end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));
  const auto programNode = pl0c::parser::run(tokens);

  const auto actualPlusExprNode =
      *std::dynamic_pointer_cast<pl0c::parser::PlusExprNode>(
          std::dynamic_pointer_cast<pl0c::parser::AssignStmtNode>(
              programNode.getBlockNode().getStatements().front())
              ->getExprNode());
  const auto expectedPlusExprNode = pl0c::parser::PlusExprNode{
      std::make_shared<pl0c::parser::IntExprNode>(1),
      std::make_shared<pl0c::parser::IdExprNode>("x")};

  ASSERT_EQ(actualPlusExprNode, expectedPlusExprNode);
}

TEST(ParserPlusExprTest, plusExprInputPlusId)
{
  const auto textString =
      std::string{"module myModule; begin i := input + x; end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));
  const auto programNode = pl0c::parser::run(tokens);

  const auto actualPlusExprNode =
      *std::dynamic_pointer_cast<pl0c::parser::PlusExprNode>(
          std::dynamic_pointer_cast<pl0c::parser::AssignStmtNode>(
              programNode.getBlockNode().getStatements().front())
              ->getExprNode());
  const auto expectedPlusExprNode = pl0c::parser::PlusExprNode{
      std::make_shared<pl0c::parser::InputExprNode>(),
      std::make_shared<pl0c::parser::IdExprNode>("x")};

  ASSERT_EQ(actualPlusExprNode, expectedPlusExprNode);
}

TEST(ParserPlusExprTest, plusExprNegativeIdPlusInteger)
{
  const auto textString =
      std::string{"module myModule; begin i := -x + 1; end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));
  const auto programNode = pl0c::parser::run(tokens);

  const auto actualPlusExprNode =
      *std::dynamic_pointer_cast<pl0c::parser::PlusExprNode>(
          std::dynamic_pointer_cast<pl0c::parser::AssignStmtNode>(
              programNode.getBlockNode().getStatements().front())
              ->getExprNode());
  const auto expectedPlusExprNode = pl0c::parser::PlusExprNode{
      std::make_shared<pl0c::parser::NegativeExprNode>(
          std::make_shared<pl0c::parser::IdExprNode>("x")),
      std::make_shared<pl0c::parser::IntExprNode>(1)};

  ASSERT_EQ(actualPlusExprNode, expectedPlusExprNode);
}

TEST(ParserPlusExprTest, nestedPlusExprPlusInteger)
{
  const auto textString =
      std::string{"module myModule; begin i := 1 + ( 2 + 3 ); end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));
  const auto programNode = pl0c::parser::run(tokens);

  const auto actualPlusExprNode =
      *std::dynamic_pointer_cast<pl0c::parser::PlusExprNode>(
          std::dynamic_pointer_cast<pl0c::parser::AssignStmtNode>(
              programNode.getBlockNode().getStatements().front())
              ->getExprNode());
  const auto expectedPlusExprNode = pl0c::parser::PlusExprNode{
      std::make_shared<pl0c::parser::IntExprNode>(1),
      std::make_shared<pl0c::parser::PlusExprNode>(
          std::make_shared<pl0c::parser::IntExprNode>(2),
          std::make_shared<pl0c::parser::IntExprNode>(3))};

  ASSERT_EQ(actualPlusExprNode, expectedPlusExprNode);
}

TEST(ParserProgramSymbolTest, programSymbol)
{
  const auto textString = std::string{"module myModule; begin end myModule."};

  const auto tokens = pl0c::lexer::run(createText(textString));
  const auto programNode = pl0c::parser::run(tokens);

  ASSERT_EQ(programNode, pl0c::parser::ProgramNode(
                             "myModule", pl0c::parser::BlockNode{{}, {}}));
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
