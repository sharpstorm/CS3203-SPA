#pragma once

#include "qps/common/PQLQuerySynonym.h"
#include "qps/common/PQLQueryResult.h"
#include "qps/common/IEvaluatable.h"
#include "qps/clauses/PatternClause.h"

class WhilePatternClause: public PatternClause {
 public:
  explicit WhilePatternClause(PQLQuerySynonym whileSynonym,
                              ClauseArgumentPtr leftArg);
  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler);

 private:
  PQLQuerySynonym whileSynonym;
};
