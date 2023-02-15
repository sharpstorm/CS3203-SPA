#include "PQLRefExtractor.h"
#include "qps/errors/QPSParserSemanticError.h"

ClauseArgument PQLRefExtractor::extractCommonRef(QueryTokenParseState* state) {
  if (state->getCurrentToken()->isType(PQL_TOKEN_UNDERSCORE)) {
    state->advanceToken();
    return ClauseArgument(CLAUSE_ARG_WILDCARD);
  } else if (!state->getCurrentToken()->isSynName()) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
  }

  PQLToken* synonym = state->expectSynName();
  PQLQuerySynonym* var = state->getQueryBuilder()
      ->getVariable(synonym->getData());
  if (var == nullptr) {
    state->setSemanticError(QPS_PARSER_ERR_UNKNOWN_SYNONYM);
    return ClauseArgument(CLAUSE_ARG_INVALID);
  }

  return ClauseArgument(*var);
}
