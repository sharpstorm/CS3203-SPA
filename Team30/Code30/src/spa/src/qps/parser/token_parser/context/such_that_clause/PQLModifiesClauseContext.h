#pragma once

#include "PQLSuchThatClauseContext.h"

class PQLModifiesClauseContext: public PQLSuchThatClauseContext {
 public:
  void parse(TokenParseState* parserState);
};
