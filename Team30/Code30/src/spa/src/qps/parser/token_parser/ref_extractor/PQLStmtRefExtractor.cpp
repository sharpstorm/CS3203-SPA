#include "PQLStmtRefExtractor.h"
#include "qps/clauses/arguments/ClauseArgumentFactory.h"
#include "qps/errors/QPSParserSemanticError.h"

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
  if (value <= 0) {
    state->getQueryBuilder()->setError(QPS_PARSER_ERR_INVALID_STMT);
  }
  return ClauseArgumentFactory::create(value);
}
