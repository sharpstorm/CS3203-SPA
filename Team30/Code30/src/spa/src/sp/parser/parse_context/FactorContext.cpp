#include <memory>
#include "FactorContext.h"
#include "../../common/ASTNode/math/PlusASTNode.h"
#include "../../common/ASTNode/math/FactorASTNode.h"

using std::shared_ptr;

bool FactorContext::validate(SourceParseState *state) {
  return true;
}

shared_ptr<ASTNode> FactorContext::generateSubtree(SourceParseState *state) {
  shared_ptr<ASTNode> node;
  shared_ptr<FactorASTNode> newNode =
      shared_ptr<FactorASTNode>(new FactorASTNode());
  shared_ptr<ASTNode> currCache = state->getCached();
  switch (state->getCurrToken()->getType()) {
    case SIMPLE_TOKEN_INTEGER:
      return contextProvider->
          getContext(CONST_CONTEXT)->generateSubtree(state);
    case SIMPLE_TOKEN_VARIABLE:
      return contextProvider->
          getContext(VARIABLE_CONTEXT)->generateSubtree(state);
    case SIMPLE_TOKEN_BRACKET_ROUND_LEFT:
      expect(state, SIMPLE_TOKEN_BRACKET_ROUND_LEFT);
      state->clearCached();
      node = contextProvider->
          getContext(EXPR_CONTEXT)->generateSubtree(state);
      expect(state, SIMPLE_TOKEN_BRACKET_ROUND_RIGHT);
      state->setCached(node);
      return node;
    default:
      throw SPError("Unknown token sequence");
  }
}
