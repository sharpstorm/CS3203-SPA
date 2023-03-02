#include "WhilePatternClause.h"

#include <utility>

WhilePatternClause::WhilePatternClause(PQLQuerySynonym whileSynonym,
                                       ClauseArgumentPtr leftArg):
    PatternClause(whileSynonym, std::move(leftArg), PQL_SYN_TYPE_WHILE) {}

PQLQueryResult *WhilePatternClause::evaluateOn(
    shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  return new PQLQueryResult();
}
