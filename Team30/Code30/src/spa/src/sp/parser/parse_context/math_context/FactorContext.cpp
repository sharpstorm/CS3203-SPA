#include <memory>
#include "FactorContext.h"
#include "common/ASTNode/math/PlusASTNode.h"
#include "common/ASTNode/math/FactorASTNode.h"

using std::shared_ptr;

shared_ptr<ASTNode> FactorContext::generateSubtree(SourceParseState *state) {
  shared_ptr<ASTNode> node;

  switch (state->getCurrToken()->getType()) {
    case SIMPLE_TOKEN_INTEGER:
      return contextProvider->
          getContext(CONST_CONTEXT)->generateSubtree(state);
    case SIMPLE_TOKEN_BRACKET_ROUND_LEFT:
      expect(state, SIMPLE_TOKEN_BRACKET_ROUND_LEFT);
      state->clearCached();
      node = contextProvider->
          getContext(EXPR_CONTEXT)->generateSubtree(state);
      expect(state, SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);
      state->clearCached();
      return node;
    case SIMPLE_TOKEN_VARIABLE:
      return contextProvider->
          getContext(VARIABLE_CONTEXT)->generateSubtree(state);
    default:
      throw SPError("Unknown token sequence");
  }
}
