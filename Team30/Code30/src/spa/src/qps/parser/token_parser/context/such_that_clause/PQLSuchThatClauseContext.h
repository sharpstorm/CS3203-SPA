#pragma once

#include "../AbstractPQLContext.h"
#include "../../../PQLToken.h"
#include "../../../../clauses/ClauseArgument.h"

class PQLSuchThatClauseContext: public AbstractPQLContext {
 public:
  virtual void parse(TokenParseState* parserState) = 0;

 protected:
  PQLSuchThatClauseContext();
  ClauseArgument extractStatementRef(TokenParseState* state);
  ClauseArgument extractEntityRef(TokenParseState* state);

 private:
  ClauseArgument extractCommonRef(TokenParseState* state);
};
