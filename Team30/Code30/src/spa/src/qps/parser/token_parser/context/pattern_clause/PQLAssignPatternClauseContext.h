#pragma once

#include "PQLPatternClauseContext.h"

class PQLAssignPatternClauseContext : PQLPatternClauseContext {
 public:
  void parse(QueryTokenParseState* parserState);
};
