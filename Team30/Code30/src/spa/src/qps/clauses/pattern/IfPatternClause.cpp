#include "IfPatternClause.h"

#include <utility>

IfPatternClause::IfPatternClause(const PQLQuerySynonym &ifSynonym,
                                 ClauseArgumentPtr leftArg):
    PatternClause(ifSynonym, std::move(leftArg), PQL_SYN_TYPE_IF) {}

PQLQueryResult *IfPatternClause::evaluateOn(PkbQueryHandler* pkbQueryHandler, OverrideTable* table) {
//PQLQueryResult *IfPatternClause::evaluateOn(PkbQueryHandler* pkbQueryHandler) {
  return new PQLQueryResult();
}
