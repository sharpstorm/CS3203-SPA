#include "IfPatternClause.h"

#include <utility>

IfPatternClause::IfPatternClause(PQLQuerySynonym ifSynonym,
                                 ClauseArgumentPtr leftArg):
    PatternClause(ifSynonym, std::move(leftArg), PQL_SYN_TYPE_IF) {}

PQLQueryResult *IfPatternClause::evaluateOn(
    shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  return new PQLQueryResult();
}
