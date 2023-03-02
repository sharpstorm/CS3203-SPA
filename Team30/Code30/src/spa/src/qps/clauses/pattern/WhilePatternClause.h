#pragma once

#include <memory>

#include "qps/common/PQLQuerySynonym.h"
#include "qps/common/PQLQueryResult.h"
#include "qps/common/IEvaluatable.h"
#include "qps/clauses/PatternClause.h"

class WhilePatternClause: public PatternClause {
 public:
  explicit WhilePatternClause(PQLQuerySynonym whileSynonym,
                              ClauseArgumentPtr leftArg);
  PQLQueryResult* evaluateOn(shared_ptr<PkbQueryHandler> pkbQueryHandler);

 private:
  PQLQuerySynonym whileSynonym;
};
