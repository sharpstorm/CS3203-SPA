#pragma once

#include "qps/common/PQLQuerySynonym.h"
#include "qps/common/PQLQueryResult.h"
#include "qps/clauses/PatternClause.h"

class IfPatternClause: public PatternClause {
 public:
  explicit IfPatternClause(const PQLQuerySynonym &ifSynonym,
                           ClauseArgumentPtr leftArg);
  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler) override;

 private:
  PQLQuerySynonym ifSynonym;
};
