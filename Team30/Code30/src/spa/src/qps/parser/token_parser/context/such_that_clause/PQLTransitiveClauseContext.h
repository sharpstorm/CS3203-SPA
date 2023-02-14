#pragma once

#include "PQLSuchThatClauseContext.h"

class PQLTransitiveClauseContext : public PQLSuchThatClauseContext {
 protected:
  bool parseTransitiveQualifier(QueryTokenParseState *parserState);
};
