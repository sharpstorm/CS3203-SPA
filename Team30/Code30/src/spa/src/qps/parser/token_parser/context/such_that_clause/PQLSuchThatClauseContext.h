#pragma once

#include "../AbstractPQLContext.h"
#include "../../../PQLToken.h"
#include "../../../../clauses/ClauseArgument.h"

class PQLSuchThatClauseContext: public AbstractPQLContext {
 public:
  virtual void parse(QueryTokenParseState* parserState) = 0;

 protected:
  PQLSuchThatClauseContext();
  ClauseArgument extractStatementRef(QueryTokenParseState* state);
  ClauseArgument extractEntityRef(QueryTokenParseState* state);
  ClauseArgument extractAnyRef(QueryTokenParseState* state);

 private:
  ClauseArgument extractCommonRef(QueryTokenParseState* state);
  ClauseArgument extractEntity(QueryTokenParseState* state);
  ClauseArgument extractStatement(QueryTokenParseState* state);
};
