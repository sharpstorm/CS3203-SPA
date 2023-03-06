#include "PQLStmtRefExtractor.h"
#include "qps/clauses/arguments/ClauseArgumentFactory.h"
#include "qps/errors/QPSParserSemanticError.h"

ClauseArgumentPtr PQLStmtRefExtractor::extract(
    QueryTokenParseState *state) {
  if (state->isCurrentTokenType(PQL_TOKEN_INTEGER)) {
    return extractStatement(state);
  }

  return extractCommonRef(state);
}

ClauseArgumentPtr PQLStmtRefExtractor::extractStatement(
    QueryTokenParseState* state) {
  PQLToken* token = state->expect(PQL_TOKEN_INTEGER);
  int value = stoi(token->getData());

  if (value <= 0) {
    state->getQueryBuilder()->setError(QPS_PARSER_ERR_INVALID_STMT);
  }
  return ClauseArgumentFactory::create(value);
}
