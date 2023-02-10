#pragma once

#include "PQLPatternClauseContext.h"

class PQLAssignPatternClauseContext : public PQLPatternClauseContext {
 public:
  void parse(QueryTokenParseState* parserState);
};
