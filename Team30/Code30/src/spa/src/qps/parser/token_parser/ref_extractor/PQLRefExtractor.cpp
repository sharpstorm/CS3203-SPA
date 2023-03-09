#include "PQLRefExtractor.h"
#include "qps/clauses/arguments/ClauseArgumentFactory.h"

ClauseArgumentPtr PQLRefExtractor::extractCommonRef(
    QueryTokenParseState* state,  QueryBuilder* builder) {
  if (state->isCurrentTokenType(PQL_TOKEN_UNDERSCORE)) {
    state->advanceToken();
    return ClauseArgumentFactory::createWildcard();
  }

  PQLSynonymName synName = state->expectSynName();
  PQLQuerySynonym* var = builder->accessSynonym(synName);
  if (var == nullptr) {
    return nullptr;
  }

  return ClauseArgumentFactory::create(*var);
}
