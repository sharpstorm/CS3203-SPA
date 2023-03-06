#pragma once

#include "AbstractSuchThatClauseContext.h"

template<class Clause, class ClauseT>
class PQLTransitiveClauseContext : public AbstractSuchThatClauseContext {
 protected:
  bool parseTransitiveQualifier(QueryTokenParseState *parserState) {
    bool isTransitive = parserState->isCurrentTokenType(PQL_TOKEN_ASTRIX);

    if (isTransitive) {
      parserState->advanceToken();
    }

    return isTransitive;
  }

  template<class LeftArgExtractor, class RightArgExtractor>
  SuchThatClausePtr parseTransitive(QueryTokenParseState *parserState,
                                    QueryBuilder* builder) {
    bool isTransitive = parseTransitiveQualifier(parserState);

    if (isTransitive) {
      return AbstractSuchThatClauseContext::parseArgs<
          ClauseT, LeftArgExtractor, RightArgExtractor>(parserState, builder);
    } else {
      return AbstractSuchThatClauseContext::parseArgs<
          Clause, LeftArgExtractor, RightArgExtractor>(parserState, builder);
    }
  }
};
