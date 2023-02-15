#pragma once

#include "../QueryTokenParseState.h"
#include "IPQLContext.h"
#include "../../../errors/QPSParserSyntaxError.h"

class AbstractPQLContext: public IPQLContext {
 public:
  virtual void parse(QueryTokenParseState* parserState) = 0;
};
