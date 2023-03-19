#include <unordered_set>

#include "WithSelectClause.h"
#include "qps/clauses/arguments/ClauseArgumentFactory.h"

using std::unordered_set;

WithSelectClause::WithSelectClause(AttributedSynonym aSyn, EntityValue enVal) :
    attrSyn(aSyn), entVal(enVal) { }

PQLQueryResult *WithSelectClause::evaluateOn(PkbQueryHandler *pkbQueryHandler,
                                             OverrideTable *table) {
  PQLQuerySynonym* syn = attrSyn.getSyn();
  ClauseArgumentPtr clauseArg = ClauseArgumentFactory::create(
      attrSyn.getSynProxy());
  PQLSynonymType synType = syn->getType();

  StmtRef stmtVar = clauseArg->toStmtRef();
  unordered_set<int> pkbResult = pkbQueryHandler
      ->getStatementsOfType(stmtVar.type);

  // read/print.varName, call.procName
  unordered_set<int> foundSet = unordered_set<int>();
  for (auto s : pkbResult) {
    if (synType == PQL_SYN_TYPE_READ &&
        pkbQueryHandler->getReadDeclarations(s) == entVal) {
      foundSet.insert(s);
    } else if (synType == PQL_SYN_TYPE_PRINT &&
        pkbQueryHandler->getPrintDeclarations(s) == entVal) {
      foundSet.insert(s);
    } else if (pkbQueryHandler->getCalledDeclaration(s) == entVal) {
      foundSet.insert(s);
    }
  }

  return Clause::toQueryResult(attrSyn.getName(), foundSet);
}

bool WithSelectClause::validateArgTypes(VariableTable *variables) {
  return true;
}

SynonymList WithSelectClause::getUsedSynonyms() {
  return SynonymList({attrSyn.getName()});
}
