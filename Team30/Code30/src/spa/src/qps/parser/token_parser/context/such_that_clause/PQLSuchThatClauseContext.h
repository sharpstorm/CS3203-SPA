#pragma once

#include "../AbstractPQLContext.h"
#include "../../../PQLToken.h"
#include "../../../../clauses/ClauseArgument.h"

class PQLSuchThatClauseContext: public AbstractPQLContext {
 protected:
  void PQLSuchThatClauseContext::parse(QueryTokenParseState *parserState);
};
