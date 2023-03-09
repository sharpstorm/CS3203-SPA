#pragma once

#include "qps/clauses/SuchThatClause.h"
#include "qps/parser/token_parser/QueryTokenParseState.h"
#include "qps/parser/builder/QueryBuilder.h"

class IPQLSuchThatClauseContext {
 public:
  virtual SuchThatClausePtr parse(QueryTokenParseState *parserState,
                                  QueryBuilder* builder) = 0;
};
