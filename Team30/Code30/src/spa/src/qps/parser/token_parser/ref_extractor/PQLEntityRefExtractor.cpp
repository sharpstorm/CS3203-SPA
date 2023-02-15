#include "PQLEntityRefExtractor.h"

ClauseArgument PQLEntityRefExtractor::extract(
    QueryTokenParseState* state) {
  if (state->getCurrentToken()->isType(PQL_TOKEN_QUOTE)) {
    return extractEntity(state);
  }

  return extractCommonRef(state);
}

ClauseArgument PQLEntityRefExtractor::extractEntity(
    QueryTokenParseState* state) {
  state->expect(PQL_TOKEN_QUOTE);
  PQLToken* entityRef = state->expectVarchar();
  state->expect(PQL_TOKEN_QUOTE);
  return ClauseArgument(entityRef->tokenData);
}
