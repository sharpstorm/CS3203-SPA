#include "IfPatternClause.h"

IfPatternClause::IfPatternClause(PQLQuerySynonym ifSynonym):
    ifSynonym(ifSynonym) {}

PQLQueryResult *IfPatternClause::evaluateOn(
    shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  return new PQLQueryResult();
}

SynonymList IfPatternClause::getUsedSynonyms() {
  return SynonymList{ ifSynonym.getName() };
}

bool IfPatternClause::validateArgTypes(VariableTable *variables) {
  return ifSynonym.isType(PQL_SYN_TYPE_IF);
}
