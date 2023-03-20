#pragma once

#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/arguments/ExpressionArgument.h"
#include "../PatternClause.h"

class AssignPatternClause: public PatternClause {
 private:
  ExpressionArgumentPtr rightArgument;

  void checkTries(const QueryExecutorAgent &agent,
                  QueryResult<StmtValue, EntityValue>* result,
                  QueryResult<StmtValue, EntityValue>* modifiesResult);

 public:
  AssignPatternClause(const PQLQuerySynonymProxy &assignSynonym,
                      ClauseArgumentPtr leftArg,
                      ExpressionArgumentPtr rightArg);
  PQLQueryResult* evaluateOn(const QueryExecutorAgent &agent) override;
};
