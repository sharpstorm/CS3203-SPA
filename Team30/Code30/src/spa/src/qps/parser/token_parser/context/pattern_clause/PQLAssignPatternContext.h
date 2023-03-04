#pragma once

#include "common/parser/ISourceExpressionParser.h"
#include "qps/clauses/PatternClause.h"
#include "IntermediateExpressionArgument.h"

class PQLAssignPatternContext {
 public:
  explicit PQLAssignPatternContext(ISourceExpressionParser* exprParser);
  PatternClausePtr parse(PQLQuerySynonym* synonym,
                         ClauseArgumentPtr firstArg,
                         IntermediateExpressionArgumentPtr secondArg);

 private:
  ISourceExpressionParser* exprParser;

  ExpressionSequencePtr buildPostfix(IAST* tree);
};
