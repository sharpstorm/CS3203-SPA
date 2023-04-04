#include <utility>
#include <memory>

#include "AssignPatternClause.h"
#include "qps/clauses/arguments/SynonymArgument.h"
#include "qps/clauses/ClauseScoring.h"
#include "common/pattern/PatternConverter.h"
#include "qps/errors/QPSParserSyntaxError.h"

using std::make_unique;

AssignPatternClause::AssignPatternClause(
    const PQLQuerySynonymProxy &assignSynonym,
    ClauseArgumentPtr leftArg,
    IASTPtr rightArg,
    bool allow):
    PatternClause(assignSynonym, std::move(leftArg), PQL_SYN_TYPE_ASSIGN),
    rightArgument(std::move(rightArg)),
    allowsPartial(allow) {}

PQLQueryResult *AssignPatternClause::evaluateOn(
    const QueryExecutorAgent &agent) {
  ExpressionArgumentPtr expr = toExpressionArg(agent);

  StmtRef leftStatement = {StmtType::Assign, 0};
  EntityRef rightVariable = leftArg->toEntityRef();

  leftStatement = agent.transformArg(synonym->getName(), leftStatement);
  rightVariable = agent.transformArg(leftArg->getName(), rightVariable);

  if (!agent.isValid(leftStatement) || !agent.isValid(rightVariable)) {
    return new PQLQueryResult();
  }

  // Override wildcards, types are ignored for known
  rightVariable.setType(EntityType::Variable);
  auto modifiesResult = agent->queryModifies(leftStatement, rightVariable);

  if (expr->isWildcard()) {
    return Clause::toQueryResult(synonym->getName(), leftArg.get(),
                                 modifiesResult.get());
  }

  auto assignResult = make_unique<QueryResult<StmtValue, EntityValue>>();
  checkTries(agent, assignResult.get(), modifiesResult.get(), expr.get());

  // Convert to PQLQueryResult
  return Clause::toQueryResult(synonym->getName(), leftArg.get(),
                               assignResult.get());
}

void AssignPatternClause::checkTries(
    const QueryExecutorAgent &agent,
    QueryResult<StmtValue, EntityValue> *output,
    QueryResult<StmtValue, EntityValue> *modifiesResult,
    ExpressionArgument* expr) {
  for (auto &it : modifiesResult->pairVals) {
    // Call assigns to retrieve the node
    StmtRef assignRef = {StmtType::Assign, it.first};
    auto nodes = agent->queryAssigns(assignRef);
    PatternTrie *lineRoot = *nodes->secondArgVals.begin();
    if (isTrieMatch(lineRoot, expr)) {
      output->add(it.first, it.second);
    }
  }
}

bool AssignPatternClause::isTrieMatch(PatternTrie *lineRoot,
                                      ExpressionArgument* expr) {
  bool isPartialMatch = expr->allowsPartial()
      && lineRoot->isMatchPartial(expr->getSequence());
  bool isFullMatch = !expr->allowsPartial()
      && lineRoot->isMatchFull(expr->getSequence());
  return isPartialMatch || isFullMatch;
}

ComplexityScore AssignPatternClause::getComplexityScore(
    const OverrideTable *table) {
  if (table->contains(leftArg->getName())) {
    return COMPLEXITY_QUERY_CONSTANT;
  }
  return COMPLEXITY_QUERY_SYN_ASSIGN;
}

ExpressionArgumentPtr AssignPatternClause::toExpressionArg(
    const QueryExecutorAgent &agent) {
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
