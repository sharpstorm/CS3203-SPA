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
  unordered_set<StmtValue> pkbResult;

  if (stmtVar.isKnown() && agent.isValid(stmtVar)) {
    pkbResult.insert(stmtVar.getValue());
  } else if (!stmtVar.isKnown()) {
    pkbResult = agent->getStatementsOfType(stmtVar.getType());
  }

  // read/print.varName, call.procName
  unordered_set<StmtValue> foundSet = {};
  if (synType == PQL_SYN_TYPE_READ) {
    queryStmtAttributes<WithSelectClause::isReadVarName>(
        agent, pkbResult, &foundSet);
  } else if (synType == PQL_SYN_TYPE_PRINT) {
    queryStmtAttributes<WithSelectClause::isPrintVarName>(
        agent, pkbResult, &foundSet);
  } else if (synType == PQL_SYN_TYPE_CALL) {
    queryStmtAttributes<WithSelectClause::isCallProcName>(
        agent, pkbResult, &foundSet);
  }

  return Clause::toQueryResult(attrSyn.getName(), foundSet);
}

bool WithSelectClause::validateArgTypes(VariableTable *variables) {
  return true;
}

SynonymList WithSelectClause::getUsedSynonyms() {
  return { attrSyn.getName() };
}

template < WithSelectClausePredicate predicate>
void WithSelectClause::queryStmtAttributes(
    const QueryExecutorAgent &agent,
    const unordered_set<StmtValue> &lines,
    unordered_set<StmtValue> *output) {
  for (StmtValue s : lines) {
    if (predicate(agent, s, entVal)) {
      output->insert(s);
    }
  }
}

bool WithSelectClause::isPrintVarName(const QueryExecutorAgent &agent,
                                      const StmtValue &stmt,
                                      const EntityValue &value) {
  return agent->getPrintDeclarations(stmt) == value;
}

bool WithSelectClause::isReadVarName(const QueryExecutorAgent &agent,
                                     const StmtValue &stmt,
                                     const EntityValue &value) {
  return agent->getReadDeclarations(stmt) == value;
}

bool WithSelectClause::isCallProcName(const QueryExecutorAgent &agent,
                                      const StmtValue &stmt,
                                      const EntityValue &value) {
  return agent->getCalledDeclaration(stmt) == value;
}
