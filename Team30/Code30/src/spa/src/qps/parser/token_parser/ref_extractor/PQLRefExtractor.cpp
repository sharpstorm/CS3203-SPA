#include "PQLRefExtractor.h"
#include "qps/clauses/arguments/ClauseArgumentFactory.h"

ClauseArgumentPtr PQLRefExtractor::extractCommonRef(
    QueryTokenParseState* state) {
  if (state->getCurrentToken()->isType(PQL_TOKEN_UNDERSCORE)) {
    state->advanceToken();
    return ClauseArgumentFactory::createWildcard();
  } else if (!state->getCurrentToken()->isSynName()) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
  }

  PQLSynonymName synName = state->expectSynName()->getData();
  PQLQuerySynonym* var = state->getQueryBuilder()->accessSynonym(synName);
  if (var == nullptr) {
    return nullptr;
  }

  return ClauseArgumentFactory::create(*var);
}
