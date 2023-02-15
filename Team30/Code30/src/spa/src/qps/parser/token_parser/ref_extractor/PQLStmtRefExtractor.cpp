#include "PQLStmtRefExtractor.h"

ClauseArgument PQLStmtRefExtractor::extract(QueryTokenParseState *state) {
  if (state->getCurrentToken()->isType(PQL_TOKEN_INTEGER)) {
    return extractStatement(state);
  }

  return extractCommonRef(state);
}

ClauseArgument PQLStmtRefExtractor::extractStatement(
    QueryTokenParseState* state) {
  int value = stoi(state->getCurrentToken()->tokenData);
  state->advanceToken();
  return ClauseArgument(value);
}
