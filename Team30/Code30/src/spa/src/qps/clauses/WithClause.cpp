#include <utility>

#include "WithClause.h"
#include "qps/clauses/arguments/ClauseArgumentFactory.h"
#include "qps/clauses/with/WithClauseEvaluator.h"
#include "ClauseScoring.h"

WithClause::WithClause(const AttributedSynonym &left,
                       const AttributedSynonym &right) :
    leftArg(std::move(left)), rightArg(std::move(right)) {}

PQLQueryResult *WithClause::evaluateOn(const QueryExecutorAgent &agent) const {
  return WithClauseEvaluator(agent, &leftArg, &rightArg).evaluate();
}

bool WithClause::validateArgTypes(const VariableTable *variables) const {
  // They both must return an int or a str
  return leftArg.returnsInteger() == rightArg.returnsInteger();
}

PQLSynonymNameList WithClause::getUsedSynonyms() const {
  return {leftArg.getName(), rightArg.getName()};
}

ComplexityScore WithClause::getComplexityScore(
    const OverrideTable *table) const {
  return COMPLEXITY_WITH;
}
