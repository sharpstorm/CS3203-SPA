#pragma once

#include "qps/parser/token_parser/QueryTokenParseState.h"
#include "qps/parser/builder/QueryBuilder.h"

class IPQLParser {
 public:
  virtual ~IPQLParser() = default;
  virtual void parse(QueryTokenParseState *parserState,
                     QueryBuilder *queryBuilder) = 0;
};
