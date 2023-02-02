#pragma once

#include "../AbstractPQLContext.h"
#include "../../../PQLToken.h"
#include "../../../../clauses/ClauseArgument.h"

class PQLSuchThatClauseContext: public AbstractPQLContext {
 public:
  void parse(TokenParseState* parserState);

 protected:
  PQLSuchThatClauseContext();
  ClauseArgument extractStatementRef(TokenParseState* state);
  ClauseArgument extractEntityRef(TokenParseState* state);

 private:
  ClauseArgument extractCommonRef(TokenParseState* state);
};
