#pragma once

#include <string>
#include "qps/parser/PQLToken.h"
#include "qps/parser/token_parser/QueryTokenParseState.h"
#include "qps/parser/builder/QueryBuilder.h"

using std::string;

class PQLSelectParser {
 public:
  void parse(QueryTokenParseState* parserState, QueryBuilder *queryBuilder);

 private:
  void parseTuple(QueryTokenParseState* parserState,
                  QueryBuilder *queryBuilder);
  void addResultSynonym(QueryBuilder *queryBuilder,
                        string synName);
};
