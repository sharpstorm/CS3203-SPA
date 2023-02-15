#pragma once

#include <memory>
#include "../AbstractPQLContext.h"
#include "../../../PQLToken.h"
#include "../../../../clauses/ClauseArgument.h"

using std::unique_ptr;

class PQLSuchThatClauseContext: public AbstractPQLContext {
 protected:
  virtual void parse(QueryTokenParseState *parserState);

  template<class Clause, class LeftArgExtractor, class RightArgExtractor>
  void parseArgs(QueryTokenParseState *parserState) {
    PQLSuchThatClauseContext::parse(parserState);

    parserState->expect(PQL_TOKEN_BRACKET_OPEN);
    ClauseArgument left = LeftArgExtractor::extract(parserState);
    parserState->expect(PQL_TOKEN_COMMA);
    ClauseArgument right = RightArgExtractor::extract(parserState);
    parserState->expect(PQL_TOKEN_BRACKET_CLOSE);

    parserState->getQueryBuilder()
        ->addSuchThat(unique_ptr<SuchThatClause>(new Clause(left, right)));
  }
};
