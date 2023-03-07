#pragma once

#include "qps/common/PQLQuerySynonym.h"
#include "common/parser/ISourceExpressionParser.h"
#include "PQLAssignPatternContext.h"
#include "IntermediateExpressionArgument.h"
#include "qps/parser/token_parser/parsers/IPQLParser.h"

class PQLPatternParser: public IPQLParser {
 public:
  explicit PQLPatternParser(ISourceExpressionParser* exprParser);
  void parse(QueryTokenParseState* parserState,
             QueryBuilder* builder) override;

 private:
  PQLAssignPatternContext assignContextParser;

  PatternClausePtr parsePatternClause(QueryTokenParseState* parserState,
                                      QueryBuilder* builder);
  PatternClausePtr extractRemainingArgs(QueryTokenParseState *parserState,
                                        PQLQuerySynonym* synonym,
                                        ClauseArgumentPtr firstArg);
  static PQLQuerySynonym* parseSynonym(QueryTokenParseState *parserState,
                                       QueryBuilder *builder);

  PatternClausePtr
  dispatchTwoArg(PQLQuerySynonym* synonym,
                 ClauseArgumentPtr firstArg,
                 IntermediateExpressionArgumentPtr secondArg);
  PatternClausePtr
  dispatchThreeArg(PQLQuerySynonym* synonym,
                   ClauseArgumentPtr firstArg,
                   IntermediateExpressionArgumentPtr secondArg);
  static IntermediateExpressionArgumentPtr
  extractExpression(QueryTokenParseState* parserState);
};
