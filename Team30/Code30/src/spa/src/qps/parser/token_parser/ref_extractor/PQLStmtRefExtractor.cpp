#include "PQLStmtRefExtractor.h"
#include "qps/clauses/arguments/ClauseArgumentFactory.h"

ClauseArgumentPtr PQLStmtRefExtractor::extract(
    QueryTokenParseState *state) {
  if (state->getCurrentToken()->isType(PQL_TOKEN_INTEGER)) {
    return extractStatement(state);
  }

  return extractCommonRef(state);
}

ClauseArgumentPtr PQLStmtRefExtractor::extractStatement(
    QueryTokenParseState* state) {
  int value = stoi(state->getCurrentToken()->getData());
  state->advanceToken();
  return ClauseArgumentFactory::create(value);
}
