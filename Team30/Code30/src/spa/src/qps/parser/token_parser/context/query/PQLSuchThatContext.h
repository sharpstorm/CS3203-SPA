#pragma once

#include "../AbstractPQLContext.h"
#include "../../../PQLToken.h"

class PQLSuchThatContext: public AbstractPQLContext {
 public:
  void parse(QueryTokenParseState* parserState);
};
