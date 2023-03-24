#include <utility>
#include <memory>

#include "AssignPatternClause.h"
#include "qps/clauses/arguments/SynonymArgument.h"
#include "qps/clauses/ClauseScoring.h"

using std::make_unique;

AssignPatternClause::AssignPatternClause(
    const PQLQuerySynonymProxy &assignSynonym,
    ClauseArgumentPtr leftArg,
    ExpressionArgumentPtr rightArg):
    PatternClause(assignSynonym, std::move(leftArg), PQL_SYN_TYPE_ASSIGN),
    rightArgument(std::move(rightArg)) {}

PQLQueryResult *AssignPatternClause::evaluateOn(
    const QueryExecutorAgent &agent) {
  StmtRef leftStatement = {StmtType::Assign, 0};
  EntityRef rightVariable = leftArg->toEntityRef();

  leftStatement = agent.transformArg(synonym->getName(), leftStatement);
  rightVariable = agent.transformArg(leftArg->getName(), rightVariable);

  if (!agent.isValid(leftStatement) || !agent.isValid(rightVariable)) {
    return new PQLQueryResult();
  }

  QueryResult<StmtValue, EntityValue> modifiesResult = agent
      ->queryModifies(leftStatement, rightVariable);

  if (rightArgument->isWildcard()) {
    return Clause::toQueryResult(synonym->getName(), leftArg.get(),
                                 modifiesResult);
  }

  QueryResult<StmtValue, EntityValue> assignResult;
  checkTries(agent, &assignResult, &modifiesResult);

  // Convert to PQLQueryResult
  return Clause::toQueryResult(synonym->getName(), leftArg.get(),
                               assignResult);
}

void AssignPatternClause::checkTries(
    const QueryExecutorAgent &agent,
    QueryResult<StmtValue, EntityValue> *output,
    QueryResult<StmtValue, EntityValue>* modifiesResult) {
  for (auto& it : modifiesResult->pairVals) {
    // Call assigns to retrieve the node
    StmtRef assignRef = {StmtType::Assign, it.first};
    QueryResult<StmtValue, PatternTrie*> nodes =
        agent->queryAssigns(assignRef);

    PatternTrie* lineRoot = *nodes.secondArgVals.begin();
    bool isPartialMatch = rightArgument->allowsPartial()
        && lineRoot->isMatchPartial(rightArgument->getSequence());
    bool isFullMatch = !rightArgument->allowsPartial()
        && lineRoot->isMatchFull(rightArgument->getSequence());
    if (isPartialMatch || isFullMatch) {
      output->add(it.first, it.second);
    }
  }
}

ComplexityScore AssignPatternClause::getComplexityScore(
    const OverrideTable &table) {
  if (table.contains(leftArg->getName())) {
    return COMPLEXITY_QUERY_CONSTANT;
  }
  return COMPLEXITY_QUERY_SYN_ASSIGN;
}
