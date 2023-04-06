#include <unordered_set>
#include <utility>
#include <vector>
#include <string>

#include "SelectClause.h"
#include "arguments/ClauseArgument.h"
#include "qps/clauses/arguments/ClauseArgumentFactory.h"
#include "qps/executor/QueryExecutorAgent.h"
#include "ClauseScoring.h"
#include "qps/common/intermediate_result/PQLQueryResultBuilder.h"

using std::pair, std::unordered_set, std::vector, std::string, std::to_string;

SelectClause::SelectClause(const PQLQuerySynonymProxy &target):
    target(target) {}

PQLQueryResult *SelectClause::evaluateOn(const QueryExecutorAgent &agent) {
  ClauseArgumentPtr clauseArg = ClauseArgumentFactory::create(target);
  PQLSynonymName synName = target->getName();
  if (target->isStatementType()) {
    StmtRef stmtRef = clauseArg->toStmtRef();
    return queryPKB<StmtValue, StmtRef, SelectClause::queryStmt>(
        agent, target->getName(), stmtRef);
  }

  EntityRef entRef = clauseArg->toEntityRef();
  return queryPKB<EntityValue , EntityRef, SelectClause::queryEntity>(
      agent, target->getName(), entRef);
}

template<class ReturnType, class RefType,
    SelectPKBGetter<ReturnType, RefType> pkbGetter>
PQLQueryResult *SelectClause::queryPKB(const QueryExecutorAgent &agent,
                                       const PQLSynonymName &synName,
                                       RefType ref) {
  unordered_set<ReturnType> result;
  ref = agent.transformArg(synName, ref);
  if (ref.isKnown() && agent.isValid(ref)) {
    result.insert(ref.getValue());
  } else if (!ref.isKnown()) {
    result = pkbGetter(agent, ref);
  }

  PQLQueryResultBuilder<ReturnType, ReturnType> builder;
  builder.setLeftName(synName);
  builder.setLeftRef(ref);
  return builder.build(result);
}

unordered_set<StmtValue> SelectClause::queryStmt(
    const QueryExecutorAgent &agent, const StmtRef &ref) {
  return agent->getStatementsOfType(ref.getType());
}

unordered_set<EntityValue> SelectClause::queryEntity(
    const QueryExecutorAgent &agent, const EntityRef &ref) {
  return agent->getSymbolsOfType(ref.getType());
}

bool SelectClause::validateArgTypes(VariableTable *variables) {
  return !target->isType(PQL_SYN_TYPE_PROCEDURE);
}

SynonymList SelectClause::getUsedSynonyms() {
  return { target->getName() };
}

ComplexityScore SelectClause::getComplexityScore(const OverrideTable *table) {
  return COMPLEXITY_SELECT;
}
