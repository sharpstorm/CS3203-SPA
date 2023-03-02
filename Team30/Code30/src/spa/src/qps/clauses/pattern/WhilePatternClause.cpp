#include "WhilePatternClause.h"

WhilePatternClause::WhilePatternClause(PQLQuerySynonym whileSynonym):
    whileSynonym(whileSynonym) {}

PQLQueryResult *WhilePatternClause::evaluateOn(
    shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  return new PQLQueryResult();
}

SynonymList WhilePatternClause::getUsedSynonyms() {
  return SynonymList{ whileSynonym.getName() };
}

bool WhilePatternClause::validateArgTypes(VariableTable *variables) {
  return whileSynonym.isType(PQL_SYN_TYPE_WHILE);
}
