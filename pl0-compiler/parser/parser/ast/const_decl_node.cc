#include "parser/ast/const_decl_node.hh"

#include "lexer/token/id_token.hh"
#include "lexer/token/integer_token.hh"

namespace pl0c
{
namespace parser
{

ConstDeclNode::ConstDeclNode(const std::string &declId,
                             const std::shared_ptr<lexer::Token> &declTypeToken,
                             const std::shared_ptr<lexer::Token> &declExprToken)
    : declId_{declId}, declTypeToken_{declTypeToken},
      declExprToken_{declExprToken}
{
}

auto ConstDeclNode::getDeclId() const -> std::string
{
  return this->declId_;
}

auto ConstDeclNode::getDeclTypeToken() const -> std::shared_ptr<lexer::Token>
{
  return this->declTypeToken_;
}

auto ConstDeclNode::getDeclExprToken() const -> std::shared_ptr<lexer::Token>
{
  return this->declExprToken_;
}

auto ConstDeclNode::getType() const -> DeclNodeType
{
  return DeclNodeType::CONST_DECL;
}

bool operator==(const ConstDeclNode &lhs, const ConstDeclNode &rhs)
{
  const bool sameDeclId = (lhs.declId_ == rhs.declId_);
  const bool sameDeclTypeToken = (*lhs.declTypeToken_ == *rhs.declTypeToken_);

  const auto sameDeclExprValue =
      [](const std::shared_ptr<const lexer::Token> &lhsExprToken,
         const std::shared_ptr<const lexer::Token> &rhsExprToken) -> bool {
    switch (lhsExprToken->getType())
    {
    case lexer::TokenType::ID: {
      const auto lhsIdToken =
          *std::dynamic_pointer_cast<const lexer::IdToken>(lhsExprToken);
      const auto rhsIdToken =
          *std::dynamic_pointer_cast<const lexer::IdToken>(rhsExprToken);
      return (lhsIdToken == rhsIdToken);
      break;
    }
    case lexer::TokenType::INT_LITERAL: {
      const auto lhsIntToken =
          *std::dynamic_pointer_cast<const lexer::IntegerToken>(lhsExprToken);
      const auto rhsIntToken =
          *std::dynamic_pointer_cast<const lexer::IntegerToken>(rhsExprToken);
      return (lhsIntToken == rhsIntToken);
      break;
    }
    default: {
      return false;
    }
    }
  }(lhs.declExprToken_, rhs.declExprToken_);

  return (sameDeclId && sameDeclTypeToken && sameDeclExprValue);
}

bool operator!=(const ConstDeclNode &lhs, const ConstDeclNode &rhs)
{
  return !(lhs == rhs);
}

}; // namespace parser
}; // namespace pl0c
