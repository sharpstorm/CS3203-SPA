#include <unordered_set>
#include <utility>
#include <vector>
#include <string>

#include "SelectClause.h"
#include "arguments/ClauseArgument.h"
#include "qps/clauses/arguments/ClauseArgumentFactory.h"
#include "qps/executor/QueryExecutorAgent.h"

using std::pair, std::unordered_set, std::vector, std::string, std::to_string;

SelectClause::SelectClause(const PQLQuerySynonymProxy &target):
    target(target) {}

PQLQueryResult *SelectClause::evaluateOn(const QueryExecutorAgent &agent) {
  ClauseArgumentPtr clauseArg = ClauseArgumentFactory::create(target);
  PQLSynonymName synName = target->getName();
  if (target->isStatementType()) {
    unordered_set<int> result;
    StmtRef stmtRef = clauseArg->toStmtRef();
    stmtRef = agent.transformArg(synName, stmtRef);
    if (stmtRef.isKnown() && agent.isValid(stmtRef)) {
      result.insert(stmtRef.lineNum);
    } else if (!stmtRef.isKnown()) {
      result = agent->getStatementsOfType(stmtRef.type);
    }

    return Clause::toQueryResult(target->getName(), result);
  }

  unordered_set<string> result;
  EntityRef entRef = clauseArg->toEntityRef();
  entRef = agent.transformArg(clauseArg->getName(), entRef);
  if (entRef.isKnown() && agent.isValid(entRef)) {
    result.insert(entRef.name);
  } else if (!entRef.isKnown()) {
    result = agent->getSymbolsOfType(entRef.type);
  }

  return Clause::toQueryResult(target->getName(), result);
}

bool SelectClause::validateArgTypes(VariableTable *variables) {
  return !target->isType(PQL_SYN_TYPE_PROCEDURE);
}

SynonymList SelectClause::getUsedSynonyms() {
  return SynonymList{target->getName()};
}
