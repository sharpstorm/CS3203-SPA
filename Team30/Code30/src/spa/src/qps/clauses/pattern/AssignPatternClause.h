#pragma once

#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/arguments/ExpressionArgument.h"
#include "../PatternClause.h"

class AssignPatternClause : public PatternClause {
 private:
  IASTPtr rightArgument;
  bool allowsPartial;

  bool isTrieMatch(PatternTrie *lineRoot, ExpressionArgument *expr) const;
  void checkTries(const QueryExecutorAgent &agent,
                  QueryResult<StmtValue, EntityValue> *output,
                  QueryResult<StmtValue, EntityValue> *modifiesResult,
                  ExpressionArgument *exprArg) const;
  ExpressionArgumentPtr toExpressionArg(const QueryExecutorAgent &agent) const;

 public:
  AssignPatternClause(const PQLQuerySynonymProxy &assignSynonym,
                      ClauseArgumentPtr leftArg,
                      IASTPtr rightArg,
                      bool allowsPartial);
  PQLQueryResult *evaluateOn(const QueryExecutorAgent &agent) const override;
  ComplexityScore getComplexityScore(const OverrideTable *table) const override;
};
