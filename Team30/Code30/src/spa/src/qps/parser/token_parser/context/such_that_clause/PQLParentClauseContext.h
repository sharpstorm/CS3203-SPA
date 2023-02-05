#pragma once

#include "PQLSuchThatClauseContext.h"

class PQLParentClauseContext: public PQLSuchThatClauseContext {
 public:
  void parse(QueryTokenParseState* parserState);
};
