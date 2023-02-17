#include "PQLRefExtractor.h"
#include "qps/errors/QPSParserSemanticError.h"

ClauseArgument PQLRefExtractor::extractCommonRef(QueryTokenParseState* state) {
  if (state->getCurrentToken()->isType(PQL_TOKEN_UNDERSCORE)) {
    state->advanceToken();
    return ClauseArgument(CLAUSE_ARG_WILDCARD);
  } else if (!state->getCurrentToken()->isSynName()) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
  }

  PQLSynonymName synName = state->expectSynName()->getData();
  PQLQuerySynonym* var = state->getQueryBuilder()->accessSynonym(synName);
  if (var == nullptr) {
    return ClauseArgument(CLAUSE_ARG_INVALID);
  }

  return ClauseArgument(*var);
}
