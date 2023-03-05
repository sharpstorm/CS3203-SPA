#pragma once

#include "qps/common/PQLQuerySynonym.h"
#include "qps/common/PQLQueryResult.h"
#include "qps/common/IEvaluatable.h"
#include "qps/clauses/PatternClause.h"

class IfPatternClause: public PatternClause {
 public:
  explicit IfPatternClause(PQLQuerySynonym ifSynonym,
                           ClauseArgumentPtr leftArg);
  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler);

 private:
  PQLQuerySynonym ifSynonym;
};
