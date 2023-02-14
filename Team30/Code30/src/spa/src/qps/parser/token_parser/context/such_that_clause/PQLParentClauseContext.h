#pragma once

#include "PQLTransitiveClauseContext.h"

class PQLParentClauseContext: public PQLTransitiveClauseContext {
 public:
  void parse(QueryTokenParseState* parserState);
};
