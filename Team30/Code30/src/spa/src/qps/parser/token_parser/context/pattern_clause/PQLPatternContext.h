#pragma once

#include "../AbstractPQLContext.h"
#include "qps/common/PQLQuerySynonym.h"
#include "qps/clauses/arguments/ExpressionArgument.h"
#include "common/parser/ISourceExpressionParser.h"

class PQLPatternContext: public AbstractPQLContext {
 public:
  explicit PQLPatternContext(ISourceExpressionParser* exprParser);
  void parse(QueryTokenParseState* parserState);

 private:
  ISourceExpressionParser* exprParser;
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
