#include "PQLRefExtractor.h"

ClauseArgument PQLRefExtractor::extractCommonRef(QueryTokenParseState* state) {
  if (state->getCurrentToken()->isType(PQL_TOKEN_UNDERSCORE)) {
    state->advanceToken();
    return ClauseArgument(CLAUSE_ARG_WILDCARD);
  } else if (!state->getCurrentToken()->isVarchar()) {
    throw QPSParserError(QPS_PARSER_ERR_UNKNOWN_TOKEN);
  }

  PQLToken* synonym = state->expectVarchar();
  PQLQuerySynonym* var = state->getQueryBuilder()
      ->getVariable(synonym->tokenData);
  if (var == nullptr) {
    throw QPSParserError(QPS_PARSER_ERR_UNKNOWN_TOKEN);
  }

  return ClauseArgument(*var);
}
