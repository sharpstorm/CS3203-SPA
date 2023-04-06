#pragma once

#include <utility>

#include "sp/ast/ASTNode.h"
#include "../../ProcedureContextType.h"
#include "../../IProcedureContextProvider.h"

template<SourceTokenType KEYWORD>
class AbstractSingleArgContext : public RecursiveProcedureParseContext {
 public:
  explicit AbstractSingleArgContext(IProcedureContextProvider *provider) :
      RecursiveProcedureParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState *state) const override;

 protected:
  virtual ASTNodePtr makeNode(const LineNumber lineNumber,
                              ASTNodePtr variableNode) const = 0;
};

template<SourceTokenType KEYWORD>
ASTNodePtr AbstractSingleArgContext<KEYWORD>::
generateSubtree(SourceParseState *state) const {
  // Keyword Entity;
  state->expect(KEYWORD);
  ASTNodePtr varNode = contextProvider->parseVariable(state);
  state->expect(SIMPLE_TOKEN_SEMICOLON);
  return makeNode(state->getLineNumber(), std::move(varNode));
}
