#pragma once

#include "../AbstractPQLContext.h"
#include "qps/common/PQLQuerySynonym.h"
#include "common/parser/ISourceExpressionParser.h"
#include "PQLAssignPatternContext.h"
#include "IntermediateExpressionArgument.h"

class PQLPatternContext: public AbstractPQLContext {
 public:
  explicit PQLPatternContext(ISourceExpressionParser* exprParser);
  void parse(QueryTokenParseState* parserState);

 private:
  PQLAssignPatternContext assignContextParser;

  void parsePatternClause(QueryTokenParseState* parserState);
  void extractRemainingArgs(QueryTokenParseState *parserState,
                            PQLQuerySynonym* synonym,
                            ClauseArgumentPtr firstArg);
  PQLQuerySynonym* parseSynonym(QueryTokenParseState* parserState);

  PatternClausePtr
  dispatchTwoArg(PQLQuerySynonym* synonym,
                 ClauseArgumentPtr firstArg,
                 IntermediateExpressionArgumentPtr secondArg);
  PatternClausePtr
  dispatchThreeArg(PQLQuerySynonym* synonym,
                   ClauseArgumentPtr firstArg,
                   IntermediateExpressionArgumentPtr secondArg);
  IntermediateExpressionArgumentPtr
  extractExpression(QueryTokenParseState* parserState);
};
