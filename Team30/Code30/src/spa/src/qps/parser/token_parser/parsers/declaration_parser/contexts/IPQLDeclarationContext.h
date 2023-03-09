#pragma once

#include "qps/parser/token_parser/QueryTokenParseState.h"
#include "qps/parser/builder/QueryBuilder.h"

class IPQLDeclarationContext {
 public:
  virtual ~IPQLDeclarationContext() = default;
  virtual void parse(QueryTokenParseState* parserState,
                     QueryBuilder *builder) = 0;
};
