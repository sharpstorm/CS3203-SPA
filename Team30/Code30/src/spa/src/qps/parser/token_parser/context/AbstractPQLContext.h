#pragma once

#include "../QueryTokenParseState.h"
#include "IPQLContext.h"
#include "../../../errors/QPSParserSyntaxError.h"

class AbstractPQLContext: public IPQLContext {
 public:
  void parse(QueryTokenParseState* parserState) override = 0;
};
