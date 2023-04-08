#pragma once

#include "qps/parser/PQLToken.h"
#include "qps/parser/token_parser/QueryTokenParseState.h"
#include "qps/parser/builder/QueryBuilder.h"

class PQLSelectParser {
 public:
  void parse(QueryTokenParseState *parserState, QueryBuilder *queryBuilder);

 private:
  static void parseTuple(QueryTokenParseState *parserState,
                         QueryBuilder *queryBuilder);
  static void parseSynonym(QueryTokenParseState *parserState,
                           QueryBuilder *queryBuilder);
};
