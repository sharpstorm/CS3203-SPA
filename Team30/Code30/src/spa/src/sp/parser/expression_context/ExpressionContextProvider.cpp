#include "ExpressionContextProvider.h"

ExpressionContextProvider::ExpressionContextProvider(
    IEntityParser *entityParser): factorContext(this, entityParser),
                                  termContext(this),
                                  exprContext(this) {
}

ASTNodePtr ExpressionContextProvider::generateSubtree(
    ExpressionContextType type,
    SourceParseState *state) const {
  return getContext(type)->generateSubtree(state);
}

const SourceParseContext *ExpressionContextProvider::getContext(
    ExpressionContextType type) const {
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
