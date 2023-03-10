#include "WhilePatternClause.h"

#include <utility>

WhilePatternClause::WhilePatternClause(const PQLQuerySynonym &whileSynonym,
                                       ClauseArgumentPtr leftArg):
    PatternClause(whileSynonym, std::move(leftArg), PQL_SYN_TYPE_WHILE) {}

PQLQueryResult *WhilePatternClause::evaluateOn(
    PkbQueryHandler* pkbQueryHandler) {
  return new PQLQueryResult();
}
