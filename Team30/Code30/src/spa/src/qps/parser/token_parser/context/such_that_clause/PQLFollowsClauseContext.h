#pragma once

#include "PQLSuchThatClauseContext.h"

class PQLFollowsClauseContext: public PQLSuchThatClauseContext {
 public:
  void parse(TokenParseState* parserState);
};
