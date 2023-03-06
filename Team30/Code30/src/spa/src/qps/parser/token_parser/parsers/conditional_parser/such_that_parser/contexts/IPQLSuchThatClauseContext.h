#pragma once

#include "qps/clauses/SuchThatClause.h"
#include "qps/parser/token_parser/QueryTokenParseState.h"

class IPQLSuchThatClauseContext {
 public:
  virtual SuchThatClausePtr parse(QueryTokenParseState *parserState,
                                  QueryBuilder* builder) = 0;
};
