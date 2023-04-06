#include "WithSelectClause.h"

#include "qps/clauses/arguments/ClauseArgumentFactory.h"
#include "ClauseScoring.h"
#include "qps/common/intermediate_result/PQLQueryResultBuilder.h"

WithSelectClause::WithSelectClause(const AttributedSynonym &aSyn,
                                   const EntityValue &enVal) :
    attrSyn(aSyn), entVal(enVal) {}

PQLQueryResult *WithSelectClause::evaluateOn(const QueryExecutorAgent &agent)
const {
  ClauseArgumentPtr clauseArg = ClauseArgumentFactory::create(
      attrSyn.getSynProxy());

  StmtRef stmtVar = clauseArg->toStmtRef();
  stmtVar = agent.transformArg(clauseArg->getName(), stmtVar);
  StmtValueSet pkbResult;

  if (stmtVar.isKnown() && agent.isValid(stmtVar)) {
    pkbResult.insert(stmtVar.getValue());
  } else if (!stmtVar.isKnown()) {
    pkbResult = agent->getStatementsOfType(stmtVar.getType());
  }

  // read/print.varName, call.procName
  StmtValueSet foundSet = {};
  projectAttribute(&foundSet, pkbResult, agent);

  PQLQueryResultBuilder<StmtValue, StmtValue> builder;
  builder.setLeftName(attrSyn.getName());
  builder.setLeftRef(stmtVar);
  return builder.build(foundSet);
}

bool WithSelectClause::validateArgTypes(const VariableTable *variables) const {
  return true;
}

const PQLSynonymNameList WithSelectClause::getUsedSynonyms() const {
  return {attrSyn.getName()};
}

template<WithSelectClausePredicate predicate>
void WithSelectClause::queryStmtAttributes(const QueryExecutorAgent &agent,
                                           const StmtValueSet &lines,
                                           StmtValueSet *output) const {
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

ComplexityScore WithSelectClause::getComplexityScore(
    const OverrideTable *table) const {
  return COMPLEXITY_WITH_SELECT;
}

void WithSelectClause::projectAttribute(StmtValueSet *output,
                                        const StmtValueSet &targets,
                                        const QueryExecutorAgent &agent) const {
  PQLSynonymType synType = attrSyn.getType();
  switch (synType) {
    case PQL_SYN_TYPE_READ:
      queryStmtAttributes<WithSelectClause::isReadVarName>(
          agent, targets, output);
      return;
    case PQL_SYN_TYPE_PRINT:
      queryStmtAttributes<WithSelectClause::isPrintVarName>(
          agent, targets, output);
      return;
    case PQL_SYN_TYPE_CALL:
      queryStmtAttributes<WithSelectClause::isCallProcName>(
          agent, targets, output);
      return;
    default:
      return;
  }
}
