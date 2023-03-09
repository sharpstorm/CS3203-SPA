#include "PQLAnyRefExtractor.h"

ClauseArgumentPtr PQLAnyRefExtractor::extract(
    QueryTokenParseState* state,
    QueryBuilder* builder) {
  if (state->isCurrentTokenType(PQL_TOKEN_STRING_LITERAL)) {
    return extractEntity(state);
  } else if (state->isCurrentTokenType(PQL_TOKEN_INTEGER)) {
    return extractStatement(state, builder);
  }

  return extractCommonRef(state, builder);
}
