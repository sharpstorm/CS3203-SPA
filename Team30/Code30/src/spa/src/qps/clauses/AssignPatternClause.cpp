#include "AssignPatternClause.h"

AssignPatternClause::AssignPatternClause(PQLQuerySynonym assignSynonym,
                                         ClauseArgument leftArgument,
                                         string patternPhrase,
                                         bool allowPartial):
    assignSynonym(assignSynonym),
    leftArgument(leftArgument),
    patternPhrase(patternPhrase),
    allowPartial(allowPartial) {}

PQLQueryResult *AssignPatternClause::evaluateOn(
    shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  return nullptr;
}

bool AssignPatternClause::usesSynonym(string varName) {
  return assignSynonym.getName() == varName
      || (leftArgument.isSynonym()
          && leftArgument.getSynonymName() == varName);
}

bool AssignPatternClause::validateArgTypes(VariableTable *variables) {
  if (!assignSynonym.isType(PQL_SYN_TYPE_ASSIGN)) {
    return false;
  }

  if (leftArgument.isSynonym()
      && leftArgument.getSynonymType() != PQL_SYN_TYPE_VARIABLE) {
    return false;
  }
  return true;
}
