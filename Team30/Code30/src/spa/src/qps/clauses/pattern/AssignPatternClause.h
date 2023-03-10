#pragma once

#include <string>

#include "qps/clauses/PatternClause.h"
#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/arguments/ExpressionArgument.h"

using std::string;

class AssignPatternClause: public PatternClause {
 private:
  ExpressionArgumentPtr rightArgument;

 public:
  AssignPatternClause(const PQLQuerySynonym &assignSynonym,
                      ClauseArgumentPtr leftArg,
                      ExpressionArgumentPtr rightArg);
  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler) override;
};
