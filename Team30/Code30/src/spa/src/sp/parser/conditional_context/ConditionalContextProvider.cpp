#include "ConditionalContextProvider.h"

ConditionalContextProvider::ConditionalContextProvider(
    IExpressionParser *exprParser): condContext(this),
                                    relContext(this),
                                    relFactorContext(exprParser) {
}

SourceParseContext
*ConditionalContextProvider::getContext(ConditionalContextType type) {
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
