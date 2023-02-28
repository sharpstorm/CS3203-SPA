#include <memory>
#include "FactorContext.h"
#include "common/ast/math/math_operand/PlusASTNode.h"

using std::shared_ptr;

shared_ptr<ASTNode> FactorContext::generateSubtree(SourceParseState *state) {
  shared_ptr<ASTNode> node;
  SourceToken* currToken = state->getCurrToken();
  if (currToken == nullptr) {
    throw SPError(SPERR_END_OF_STREAM);
  }

  if (currToken->isVarchar()) {
    return contextProvider->
        getContext(VARIABLE_CONTEXT)->generateSubtree(state);
  }

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
    default:
      throw SPError(SPERR_UNEXPECTED_TOKEN);
  }
}
