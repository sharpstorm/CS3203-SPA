#include "PQLStmtRefExtractor.h"
#include "qps/clauses/arguments/ClauseArgumentFactory.h"
#include "qps/errors/QPSParserSemanticError.h"

ClauseArgumentPtr PQLStmtRefExtractor::extract(
    QueryTokenParseState *state, QueryBuilder* builder) {
  if (state->isCurrentTokenType(PQL_TOKEN_INTEGER)) {
    return extractStatement(state, builder);
  }

  return extractCommonRef(state, builder);
}

ClauseArgumentPtr PQLStmtRefExtractor::extractStatement(
    QueryTokenParseState* state,
    QueryBuilder* builder) {
  PQLToken* token = state->expect(PQL_TOKEN_INTEGER);
  int value = stoi(token->getData());

  if (value <= 0) {
    builder->setError(QPS_PARSER_ERR_INVALID_STMT);
  }
  return ClauseArgumentFactory::create(value);
}
