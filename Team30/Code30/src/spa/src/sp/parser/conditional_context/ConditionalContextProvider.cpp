#include "ConditionalContextProvider.h"

ConditionalContextProvider::ConditionalContextProvider(
    IExpressionParser *exprParser) : condContext(this),
                                     relContext(this),
                                     relFactorContext(exprParser) {
}

ASTNodePtr ConditionalContextProvider::generateSubtree(
    ConditionalContextType type,
    SourceParseState *state) const {
  return getContext(type)->generateSubtree(state);
}

const SourceParseContext *ConditionalContextProvider::getContext(
    ConditionalContextType type) const {
  switch (type) {
    case ConditionalContextType::COND_CONTEXT:
      return &condContext;
    case ConditionalContextType::REL_CONTEXT:
      return &relContext;
    case ConditionalContextType::REL_FACTOR_CONTEXT:
      return &relFactorContext;
    default:
      throw SPError(SPERR_CONTEXT_ERROR);
  }
}
