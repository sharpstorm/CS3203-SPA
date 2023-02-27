#include "PQLEntityRefExtractor.h"
#include "qps/clauses/arguments/ClauseArgumentFactory.h"

ClauseArgumentPtr PQLEntityRefExtractor::extract(
    QueryTokenParseState* state) {
  if (state->getCurrentToken()->isType(PQL_TOKEN_STRING_LITERAL)) {
    return extractEntity(state);
  }

  return extractCommonRef(state);
}

ClauseArgumentPtr PQLEntityRefExtractor::extractEntity(
    QueryTokenParseState* state) {
  PQLToken* entityRef = state->expect(PQL_TOKEN_STRING_LITERAL);
  return ClauseArgumentFactory::create(entityRef->getData());
}
