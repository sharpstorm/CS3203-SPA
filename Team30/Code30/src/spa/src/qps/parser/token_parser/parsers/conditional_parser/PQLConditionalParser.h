#pragma once

#include "../IPQLParser.h"
#include "pattern_clause/PQLPatternParser.h"
#include "such_that_parser/PQLSuchThatParser.h"
#include "with_clause/PQLWithParser.h"

class PQLConditionalParser: public IPQLParser {
 public:
  explicit PQLConditionalParser(ISourceExpressionParser* exprParser);
  void parse(QueryTokenParseState* parserState,
             QueryBuilder *queryBuilder) override;

 private:
  IPQLParser* getParseContext(PQLTokenType type);

  PQLPatternParser patternParser;
  PQLSuchThatParser suchThatParser;
  PQLWithParser withParser;
};
