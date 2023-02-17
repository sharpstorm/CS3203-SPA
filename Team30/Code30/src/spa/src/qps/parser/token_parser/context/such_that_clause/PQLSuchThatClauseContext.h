#pragma once

#include <memory>
#include <utility>
#include "../AbstractPQLContext.h"
#include "../../../PQLToken.h"
#include "../../../../clauses/arguments/ClauseArgument.h"

using std::unique_ptr, std::move;

class PQLSuchThatClauseContext: public AbstractPQLContext {
 protected:
  virtual void parse(QueryTokenParseState *parserState);

  template<class Clause, class LeftArgExtractor, class RightArgExtractor>
  void parseArgs(QueryTokenParseState *parserState) {
    PQLSuchThatClauseContext::parse(parserState);

    parserState->expect(PQL_TOKEN_BRACKET_OPEN);
    ClauseArgumentPtr left = LeftArgExtractor::extract(parserState);
    parserState->expect(PQL_TOKEN_COMMA);
    ClauseArgumentPtr right = RightArgExtractor::extract(parserState);
    parserState->expect(PQL_TOKEN_BRACKET_CLOSE);

    if (left != nullptr && right != nullptr) {
      parserState->getQueryBuilder()
          ->addSuchThat(unique_ptr<SuchThatClause>(
              new Clause(move(left), move(right))));
    }
  }
};
