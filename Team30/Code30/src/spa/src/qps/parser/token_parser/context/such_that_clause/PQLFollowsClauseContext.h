#pragma once

#include "PQLTransitiveClauseContext.h"

class PQLFollowsClauseContext: public PQLTransitiveClauseContext {
 public:
  void parse(QueryTokenParseState* parserState);
};
