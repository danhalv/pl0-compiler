#include "analyzer/analyzer.hh"

#include "parser/ast/block_node.hh"
#include "parser/ast/const_decl_node.hh"
#include "parser/ast/decl_node.hh"
#include "parser/ast/proc_decl_node.hh"
#include "parser/ast/stmt_node.hh"
#include "parser/ast/var_decl_node.hh"

#include <cassert>
#include <memory>
#include <set>
#include <string>

namespace pl0c
{
namespace analyzer
{

struct AnalyzerContext
{
  AnalyzerContext()
      : isProcedureScope{false}, globalConstants{}, globalVariables{},
        procedureDeclarations{}, localConstants{}, localVariables{}
  {
  }

  auto idExistsLocally(const std::string &id) -> bool
  {
    return ((localConstants.find(id) != localConstants.end()) ||
            (localVariables.find(id) != localVariables.end()) ||
            (procedureDeclarations.find(id) != procedureDeclarations.end()));
  }

  auto idExistsGlobally(const std::string &id) -> bool
  {
    return ((globalConstants.find(id) != globalConstants.end()) ||
            (globalVariables.find(id) != globalVariables.end()) ||
            (procedureDeclarations.find(id) != procedureDeclarations.end()));
  }

  auto idExists(const std::string &id) -> bool
  {
    return ((globalConstants.find(id) != globalConstants.end()) ||
            (globalVariables.find(id) != globalVariables.end()) ||
            (procedureDeclarations.find(id) != procedureDeclarations.end()) ||
            (localConstants.find(id) != localConstants.end()) ||
            (localVariables.find(id) != localVariables.end()));
  }

  bool isProcedureScope;
  std::set<std::string> globalConstants;
  std::set<std::string> globalVariables;
  std::set<std::string> procedureDeclarations;
  std::set<std::string> localConstants;
  std::set<std::string> localVariables;
};

auto analyze(const std::shared_ptr<parser::DeclNode> declNode,
             AnalyzerContext &ctx) -> void
{
  switch (declNode->getType())
  {
  case parser::DeclNodeType::CONST_DECL: {
    const auto constDeclNode =
        std::dynamic_pointer_cast<parser::ConstDeclNode>(declNode);
    const auto id = constDeclNode->getDeclId();

    if (ctx.isProcedureScope)
    {
      if (ctx.idExistsLocally(id))
      {
        const auto errMsg =
            std::string{"analyzer error: local variable id: " + id +
                        ", is already declared."};
        assert(errMsg.c_str() && false);
      }

      ctx.localVariables.emplace(id);
    }
    else
    {
      if (ctx.idExistsGlobally(id))
      {
        const auto errMsg =
            std::string{"analyzer error: global variable id: " + id +
                        ", is already declared."};
        assert(errMsg.c_str() && false);
      }

      ctx.globalVariables.emplace(id);
    }

    break;
  }
  case parser::DeclNodeType::PROC_DECL: {
    const auto procDeclNode =
        std::dynamic_pointer_cast<parser::ProcDeclNode>(declNode);
    const auto id = procDeclNode->getDeclId();

    if (ctx.idExists(id))
    {
      const auto errMsg = std::string{"analyzer error: procedure id: " + id +
                                      ", is already declared."};
      assert(errMsg.c_str() && false);
    }

    ctx.procedureDeclarations.emplace(id);

    break;
  }
  case parser::DeclNodeType::VAR_DECL: {
    const auto varDeclNode =
        std::dynamic_pointer_cast<parser::VarDeclNode>(declNode);
    const auto id = varDeclNode->getDeclId();

    if (ctx.isProcedureScope)
    {
      if (ctx.idExistsLocally(id))
      {
        const auto errMsg =
            std::string{"analyzer error: local variable id: " + id +
                        ", is already declared."};
        assert(errMsg.c_str() && false);
      }

      ctx.localVariables.emplace(id);
    }
    else
    {
      if (ctx.idExistsGlobally(id))
      {
        const auto errMsg =
            std::string{"analyzer error: global variable id: " + id +
                        ", is already declared."};
        assert(errMsg.c_str() && false);
      }

      ctx.globalVariables.emplace(id);
    }

    break;
  }
  }
}

auto analyze(const parser::BlockNode &blockNode, AnalyzerContext &ctx) -> void
{
  for (const auto &declaration : blockNode.getDeclarations())
    analyze(declaration, ctx);
}

auto run(const parser::ProgramNode &programNode) -> void
{
  AnalyzerContext ctx;

  analyze(programNode.getBlockNode(), ctx);
}

}; // namespace analyzer
}; // namespace pl0c
