#include "RelationalFactorContext.h"

bool RelationalFactorContext::validate(SourceParseState *state) {
  return true;
}

shared_ptr<ASTNode> RelationalFactorContext::generateSubtree(SourceParseState *state) {
  return contextProvider->getContext(EXPR_CONTEXT)->generateSubtree(state);
}