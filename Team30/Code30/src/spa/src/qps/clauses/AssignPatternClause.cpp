#include "AssignPatternClause.h"
#include <utility>

using std::move;

AssignPatternClause::AssignPatternClause(PQLQuerySynonym assignSynonym,
                                         ClauseArgumentPtr leftArgument,
                                         string patternPhrase,
                                         bool allowPartial):
    assignSynonym(assignSynonym),
    leftArgument(move(leftArgument)),
    patternPhrase(patternPhrase),
    allowPartial(allowPartial) {}

PQLQueryResult *AssignPatternClause::evaluateOn(
    shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  return nullptr;
}

bool AssignPatternClause::usesSynonym(string varName) {
  return assignSynonym.getName() == varName
      || leftArgument->isSynonymCalled(varName);
}

bool AssignPatternClause::validateArgTypes(VariableTable *variables) {
  if (!assignSynonym.isType(PQL_SYN_TYPE_ASSIGN)) {
    return false;
  }

  return leftArgument->synonymSatisfies(
      ClauseArgument::isType<PQL_SYN_TYPE_VARIABLE>);
}
