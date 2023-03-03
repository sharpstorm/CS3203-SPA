#include "ConditionalContextProvider.h"

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
