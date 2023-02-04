#include "TermContext.h"
#include "../../common/ASTNode/math/PlusASTNode.h"

bool TermContext::validate(SourceParseState *state) {
  return true;
}

shared_ptr<ASTNode> TermContext::generateSubtree(SourceParseState *state) {
  if (state->nextTokenIsOfType(SIMPLE_TOKEN_TIMES)) {

  } else if (state->nextTokenIsOfType(SIMPLE_TOKEN_DIV)) {

  } else if (state->nextTokenIsOfType(SIMPLE_TOKEN_MOD)) {

  } else {
    return contextProvider->getContext(FACTOR_CONTEXT)->generateSubtree(state);
  }
}

