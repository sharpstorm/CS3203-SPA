#pragma once

#include "../AbstractPQLContext.h"
#include "../../../PQLToken.h"

class PQLSelectContext: public AbstractPQLContext {
 public:
  void parse(TokenParseState* parserState);
};
