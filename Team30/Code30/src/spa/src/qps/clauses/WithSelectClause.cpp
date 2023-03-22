#include <unordered_set>

#include "WithSelectClause.h"
#include "qps/clauses/arguments/ClauseArgumentFactory.h"

using std::unordered_set;

WithSelectClause::WithSelectClause(AttributedSynonym aSyn, EntityValue enVal) :
    attrSyn(aSyn), entVal(enVal) { }

PQLQueryResult *WithSelectClause::evaluateOn(const QueryExecutorAgent &agent) {
  ClauseArgumentPtr clauseArg = ClauseArgumentFactory::create(
      attrSyn.getSynProxy());
  PQLSynonymType synType = attrSyn.getType();

  StmtRef stmtVar = clauseArg->toStmtRef();
  stmtVar = agent.transformArg(clauseArg->getName(), stmtVar);
  unordered_set<int> pkbResult;
  if (stmtVar.isKnown()) {
    if (agent.isValid(stmtVar)) {
      pkbResult.insert(stmtVar.getValue());
    }
  } else if (!stmtVar.isKnown()) {
    pkbResult = agent->getStatementsOfType(stmtVar.getType());
  }

  // read/print.varName, call.procName
  unordered_set<int> foundSet = unordered_set<int>();
  for (auto s : pkbResult) {
    if (synType == PQL_SYN_TYPE_READ &&
        agent->getReadDeclarations(s) == entVal) {
      foundSet.insert(s);
    } else if (synType == PQL_SYN_TYPE_PRINT &&
        agent->getPrintDeclarations(s) == entVal) {
      foundSet.insert(s);
    } else if (agent->getCalledDeclaration(s) == entVal) {
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
