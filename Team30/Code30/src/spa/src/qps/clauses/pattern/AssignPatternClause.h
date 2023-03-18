#pragma once

#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/arguments/ExpressionArgument.h"
#include "../PatternClause.h"

class AssignPatternClause: public PatternClause {
 private:
  ExpressionArgumentPtr rightArgument;

 public:
  AssignPatternClause(const PQLQuerySynonym &assignSynonym,
                      ClauseArgumentPtr leftArg,
                      ExpressionArgumentPtr rightArg);
  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler) override;
};
