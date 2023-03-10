#include "FactorContext.h"

ASTNodePtr FactorContext::generateSubtree(SourceParseState *state) {
  ASTNodePtr node;
  SourceToken* currToken = state->getCurrToken();
  if (currToken == nullptr) {
    throw SPError(SPERR_END_OF_STREAM);
  }

  if (currToken->isVarchar()) {
    return entityParser->parseVariable(state);
  }

  switch (state->getCurrToken()->getType()) {
    case SIMPLE_TOKEN_INTEGER:
      return entityParser->parseConstant(state);
    case SIMPLE_TOKEN_BRACKET_ROUND_LEFT:
      state->expect(SIMPLE_TOKEN_BRACKET_ROUND_LEFT);
      node = contextProvider
          ->generateSubtree(ExpressionContextType::EXPR_CONTEXT, state);
      state->expect(SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);
      return node;
    default:
      throw SPError(SPERR_UNEXPECTED_TOKEN);
  }
}
