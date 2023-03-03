#pragma once

#include "sp/ast/ASTNode.h"
#include "../../ProcedureContextType.h"
#include "../../IProcedureContextProvider.h"

template <SourceTokenType KEYWORD>
class AbstractSingleArgContext: public RecursiveProcedureParseContext {
 public:
  explicit AbstractSingleArgContext(IProcedureContextProvider* provider):
      RecursiveProcedureParseContext(provider) {}
  ASTNodePtr generateSubtree(SourceParseState* state);

 protected:
  virtual ASTNodePtr makeNode(const int &lineNumber,
                              ASTNodePtr variableNode) = 0;
};

template <SourceTokenType KEYWORD>
ASTNodePtr AbstractSingleArgContext<KEYWORD>::
generateSubtree(SourceParseState* state) {
  // Keyword Entity;
  state->expect(KEYWORD);
  ASTNodePtr varNode = contextProvider
      ->getEntityParser()
      ->parseVariable(state);

  state->expect(SIMPLE_TOKEN_SEMICOLON);

  ASTNodePtr newNode = makeNode(state->getLineNumber(),
                                varNode);
  state->setCached(newNode);

  return newNode;
}
