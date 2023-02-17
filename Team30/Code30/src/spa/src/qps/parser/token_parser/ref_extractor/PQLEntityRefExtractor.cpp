#include "PQLEntityRefExtractor.h"
#include "qps/clauses/arguments/ClauseArgumentFactory.h"

ClauseArgumentPtr PQLEntityRefExtractor::extract(
    QueryTokenParseState* state) {
  if (state->getCurrentToken()->isType(PQL_TOKEN_QUOTE)) {
    return extractEntity(state);
  }

  return extractCommonRef(state);
}

ClauseArgumentPtr PQLEntityRefExtractor::extractEntity(
    QueryTokenParseState* state) {
  state->expect(PQL_TOKEN_QUOTE);
  PQLToken* entityRef = state->expectVarchar();
  state->expect(PQL_TOKEN_QUOTE);
  return ClauseArgumentFactory::create(entityRef->getData());
}
