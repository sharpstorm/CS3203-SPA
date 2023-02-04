#include "RelationalFactorContext.h"

bool RelationalFactorContext::validate(SourceParseState *state) {
  return true;
}

shared_ptr<ASTNode> RelationalFactorContext::generateSubtree(SourceParseState *state) {
  switch (state->getCurrToken()->getType()) {
    case SIMPLE_TOKEN_INTEGER:
      return contextProvider->getContext(CONST_CONTEXT)->generateSubtree(state);
    case SIMPLE_TOKEN_VARIABLE:
      return contextProvider->getContext(VARIABLE_CONTEXT)->generateSubtree(state);
    default:
      return contextProvider->getContext(EXPR_CONTEXT)->generateSubtree(state);
  }
}