#pragma once

#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/arguments/ExpressionArgument.h"
#include "../PatternClause.h"

class AssignPatternClause : public PatternClause {
 private:
  IASTPtr rightArgument;
  bool allowsPartial;

  ExpressionArgumentPtr toExpressionArg(const QueryExecutorAgent &agent);

 public:
  AssignPatternClause(const PQLQuerySynonymProxy &assignSynonym,
                      ClauseArgumentPtr leftArg,
                      IASTPtr rightArg,
                      bool allowsPartial);
  PQLQueryResult *evaluateOn(const QueryExecutorAgent &agent) override;
  ComplexityScore getComplexityScore(const OverrideTable *table) override;
};
