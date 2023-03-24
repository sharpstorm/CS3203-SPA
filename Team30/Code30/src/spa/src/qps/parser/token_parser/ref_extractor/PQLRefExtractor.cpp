#include "PQLRefExtractor.h"

#include "qps/clauses/arguments/ClauseArgumentFactory.h"

ClauseArgumentPtr PQLRefExtractor::extractCommonRef(QueryTokenParseState* state,
                                                    QueryBuilder* builder) {
  PQLToken* token = state->tryExpect(PQL_TOKEN_UNDERSCORE);

  if (token != nullptr) {
    return ClauseArgumentFactory::createWildcard();
  }

  PQLSynonymName synName = state->expectSynName();
  PQLQuerySynonymProxy* var = builder->accessSynonym(synName);

  if (var == nullptr) {
    return nullptr;
  }

  return ClauseArgumentFactory::create(*var);
}
