#pragma once

#include "../QueryTokenParseState.h"
#include "IPQLContext.h"
#include "../../../errors/QPSParserError.h"

class AbstractPQLContext: public IPQLContext {
 public:
  virtual void parse(QueryTokenParseState* parserState) = 0;

 protected:
  ClauseArgument extractStatementRef(QueryTokenParseState* state);
  ClauseArgument extractEntityRef(QueryTokenParseState* state);
  ClauseArgument extractAnyRef(QueryTokenParseState* state);

 private:
  ClauseArgument extractCommonRef(QueryTokenParseState* state);
  ClauseArgument extractEntity(QueryTokenParseState* state);
  ClauseArgument extractStatement(QueryTokenParseState* state);
};
