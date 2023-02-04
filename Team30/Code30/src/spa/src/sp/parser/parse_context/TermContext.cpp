#include "TermContext.h"
#include "../../common/ASTNode/math/PlusASTNode.h"

bool TermContext::validate(SourceParseState *state) {
  return true;
}

shared_ptr<ASTNode> TermContext::generateSubtree(SourceParseState *state) {
  if (state->nextTokenIsOfType(SIMPLE_TOKEN_TIMES)) {
    return nullptr;
  } else if (state->nextTokenIsOfType(SIMPLE_TOKEN_DIV)) {
    return nullptr;
  } else if (state->nextTokenIsOfType(SIMPLE_TOKEN_MOD)) {
    return nullptr;
  } else {
    return contextProvider->getContext(FACTOR_CONTEXT)->generateSubtree(state);
  }
}

