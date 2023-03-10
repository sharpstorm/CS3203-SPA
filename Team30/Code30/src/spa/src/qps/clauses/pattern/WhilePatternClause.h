#pragma once

#include "qps/common/PQLQuerySynonym.h"
#include "qps/common/PQLQueryResult.h"
#include "qps/clauses/PatternClause.h"

class WhilePatternClause: public PatternClause {
 public:
  explicit WhilePatternClause(const PQLQuerySynonym &whileSynonym,
                              ClauseArgumentPtr leftArg);
  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler) override;

 private:
  PQLQuerySynonym whileSynonym;
};
