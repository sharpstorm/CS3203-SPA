#include "ExpressionContextProvider.h"

SourceParseContext
*ExpressionContextProvider::getContext(ExpressionContextType type) {
  switch (type) {
    case ExpressionContextType::EXPR_CONTEXT:
      return &exprContext;
    case ExpressionContextType::TERM_CONTEXT:
      return &termContext;
    case ExpressionContextType::FACTOR_CONTEXT:
      return &factorContext;
    default:
      throw SPError(SPERR_CONTEXT_ERROR);
  }
}
