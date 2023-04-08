#include "SelectClause.h"
#include "arguments/ClauseArgument.h"
#include "qps/clauses/arguments/ClauseArgumentFactory.h"
#include "qps/executor/QueryExecutorAgent.h"
#include "ClauseScoring.h"
#include "qps/common/intermediate_result/PQLQueryResultBuilder.h"

SelectClause::SelectClause(const PQLQuerySynonymProxy &target) :
    target(target) {}

PQLQueryResult *SelectClause::evaluateOn(
    const QueryExecutorAgent &agent) const {
  ClauseArgumentPtr clauseArg = ClauseArgumentFactory::create(target);
  const PQLSynonymName synName = target->getName();
  if (target->isStatementType()) {
    StmtRef stmtRef = clauseArg->toStmtRef();
    return queryPKB<StmtValue, StmtRef, SelectClause::queryStmt>(
        agent, target->getName(), stmtRef);
  }

  EntityRef entRef = clauseArg->toEntityRef();
  return queryPKB<EntityValue, EntityRef, SelectClause::queryEntity>(
      agent, target->getName(), entRef);
}

template<class ReturnType, class RefType,
    SelectPKBGetter<ReturnType, RefType> pkbGetter>
PQLQueryResult *SelectClause::queryPKB(const QueryExecutorAgent &agent,
                                       const PQLSynonymName &synName,
                                       const RefType &ref) const {
  QueryResultSet<ReturnType> result;
  const RefType transformed = agent.transformArg(synName, ref);
  if (transformed.isKnown() && agent.isValid(transformed)) {
    result.insert(transformed.getValue());
  } else if (!transformed.isKnown()) {
    result = pkbGetter(agent, transformed);
  }

  PQLQueryResultBuilder<ReturnType, ReturnType> builder;
  builder.setLeftName(synName);
  builder.setLeftRef(transformed);
  return builder.build(result);
}

StmtValueSet SelectClause::queryStmt(const QueryExecutorAgent &agent,
                                     const StmtRef &ref) {
  return agent->getStatementsOfType(ref.getType());
}

EntityValueSet SelectClause::queryEntity(const QueryExecutorAgent &agent,
                                         const EntityRef &ref) {
  return agent->getSymbolsOfType(ref.getType());
}

bool SelectClause::validateArgTypes(const VariableTable *variables) const {
  return !target->isType(PQL_SYN_TYPE_PROCEDURE);
}

PQLSynonymNameList SelectClause::getUsedSynonyms() const {
  return {target->getName()};
}

ComplexityScore SelectClause::getComplexityScore(
    const OverrideTable *table) const {
  return COMPLEXITY_SELECT;
}
