#pragma once

#include "../AbstractPQLContext.h"
#include "qps/common/PQLQuerySynonym.h"
#include "qps/clauses/arguments/ExpressionArgument.h"

class PQLPatternContext: public AbstractPQLContext {
 public:
  void parse(QueryTokenParseState* parserState);

 private:
  void parsePatternClause(QueryTokenParseState* parserState);
  void extractRemainingArgs(QueryTokenParseState *parserState,
                            PQLQuerySynonym* synonym,
                            ClauseArgumentPtr firstArg);
  PQLQuerySynonym* parseSynonym(QueryTokenParseState* parserState);
  PatternClausePtr dispatchTwoArg(PQLQuerySynonym* synonym,
                                  ClauseArgumentPtr firstArg,
                                  ExpressionArgumentPtr secondArg);
  PatternClausePtr dispatchThreeArg(PQLQuerySynonym* synonym,
                                    ClauseArgumentPtr firstArg,
                                    ExpressionArgumentPtr secondArg);

  ExpressionArgumentPtr extractExpression(QueryTokenParseState* parserState);
};
