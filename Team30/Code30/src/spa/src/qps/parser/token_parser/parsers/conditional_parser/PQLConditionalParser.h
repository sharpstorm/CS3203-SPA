#pragma once

#include "../IPQLParser.h"
#include "pattern_clause/PQLPatternParser.h"
#include "such_that_parser/PQLSuchThatParser.h"

class PQLConditionalParser: public IPQLParser {
 public:
  explicit PQLConditionalParser(ISourceExpressionParser* exprParser);
  void parse(QueryTokenParseState* parserState, QueryBuilder *queryBuilder);

 private:
  IPQLParser* getParseContext(PQLTokenType type);

  PQLPatternParser patternParser;
  PQLSuchThatParser suchThatParser;
};
