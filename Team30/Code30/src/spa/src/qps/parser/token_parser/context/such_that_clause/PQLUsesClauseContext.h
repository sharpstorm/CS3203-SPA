#pragma once

#include "PQLSuchThatClauseContext.h"

class PQLUsesClauseContext: public PQLSuchThatClauseContext {
 public:
  void parse(QueryTokenParseState* parserState);
};