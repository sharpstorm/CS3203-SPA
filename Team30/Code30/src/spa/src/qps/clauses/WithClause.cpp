#include <utility>

#include "WithClause.h"
#include "qps/clauses/arguments/ClauseArgumentFactory.h"
#include "qps/clauses/with/WithClauseEvaluator.h"
#include "ClauseScoring.h"

WithClause::WithClause(const AttributedSynonym &left,
                       const AttributedSynonym &right) :
    leftArg(std::move(left)), rightArg(std::move(right)) {}

PQLQueryResult *WithClause::evaluateOn(const QueryExecutorAgent &agent) {
  return WithClauseEvaluator(agent, &leftArg, &rightArg).evaluate();
}

bool WithClause::validateArgTypes(VariableTable *variables) {
  bool leftRetInt = leftArg.returnsInteger();
  bool rightRetInt = rightArg.returnsInteger();

  // They both must return an int or a str
  return leftRetInt == rightRetInt;
}

SynonymList WithClause::getUsedSynonyms() {
  return {leftArg.getName(), rightArg.getName()};
}

ComplexityScore WithClause::getComplexityScore(const OverrideTable *table) {
  return COMPLEXITY_WITH;
}
