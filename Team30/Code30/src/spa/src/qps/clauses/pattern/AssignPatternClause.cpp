#include <utility>
#include <memory>

#include "AssignPatternClause.h"
#include "qps/clauses/ClauseScoring.h"
#include "common/pattern/PatternConverter.h"
#include "qps/common/intermediate_result/PQLQueryResultBuilder.h"
#include "AssignPatternChecker.h"

using std::make_unique;

AssignPatternClause::AssignPatternClause(
    const PQLQuerySynonymProxy &assignSynonym,
    ClauseArgumentPtr leftArg,
    IASTPtr rightArg,
    bool allowsPartial) :
    PatternClause(assignSynonym, std::move(leftArg), PQL_SYN_TYPE_ASSIGN),
    rightArgument(std::move(rightArg)),
    allowsPartial(allowsPartial) {}

PQLQueryResult *AssignPatternClause::evaluateOn(
    const QueryExecutorAgent &agent) const {
  ExpressionArgumentPtr expr = toExpressionArg(agent);

  StmtRef leftStatement = {StmtType::Assign, NO_STMT};
  EntityRef rightVariable = leftArg->toEntityRef();

  leftStatement = agent.transformArg(synonym->getName(), leftStatement);
  rightVariable = agent.transformArg(leftArg->getName(), rightVariable);

  if (!agent.isValid(leftStatement) || !agent.isValid(rightVariable)) {
    return new PQLQueryResult();
  }

  QueryResultPtr<StmtValue, EntityValue> modifiesResult =
      agent->queryModifies(leftStatement, rightVariable);

  PQLQueryResultBuilder<StmtValue, EntityValue> builder;
  builder.setLeftName(synonym->getName());
  builder.setRightName(leftArg.get());
  builder.setLeftRef(leftStatement);
  builder.setRightRef(rightVariable);

  if (expr->isWildcard()) {
    return builder.build(modifiesResult.get());
  }

  QueryResult<StmtValue, EntityValue> assignResult;
  AssignPatternChecker checker(expr.get(), &leftStatement, &rightVariable);
  checker.filterModifiesInto(modifiesResult.get(), &assignResult, agent);

  // Convert to PQLQueryResult
  return builder.build(&assignResult);
}

ComplexityScore AssignPatternClause::getComplexityScore(
    const OverrideTable *table) const {
  if (table->contains(leftArg->getName())) {
    return COMPLEXITY_QUERY_CONSTANT;
  }
  return COMPLEXITY_QUERY_SYN_ASSIGN;
}

ExpressionArgumentPtr AssignPatternClause::toExpressionArg(
    const QueryExecutorAgent &agent) const {
  // Wildcard case
  if (rightArgument.get() == nullptr) {
    return make_unique<ExpressionArgument>();
  }

  ExpressionSequencePtr exprSeq = PatternConverter::convertASTToPostfix(
      rightArgument.get(), agent);

  ExpressionArgumentPtr expr = make_unique<ExpressionArgument>(
      std::move(exprSeq), allowsPartial);

  return expr;
}
