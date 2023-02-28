#include "PQLAnyRefExtractor.h"

ClauseArgumentPtr PQLAnyRefExtractor::extract(
    QueryTokenParseState* state) {
  if (state->getCurrentToken()->isType(PQL_TOKEN_STRING_LITERAL)) {
    return extractEntity(state);
  } else if (state->getCurrentToken()->isType(PQL_TOKEN_INTEGER)) {
    return extractStatement(state);
  }

  return extractCommonRef(state);
}
