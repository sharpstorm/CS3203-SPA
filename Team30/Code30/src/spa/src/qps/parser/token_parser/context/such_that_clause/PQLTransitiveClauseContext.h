#pragma once

#include "PQLSuchThatClauseContext.h"

template<class Clause, class ClauseT>
class PQLTransitiveClauseContext : public PQLSuchThatClauseContext {
 protected:
  bool parseTransitiveQualifier(QueryTokenParseState *parserState) {
    bool isTransitive = parserState->isCurrentTokenType(PQL_TOKEN_ASTRIX);

    if (isTransitive) {
      parserState->advanceToken();
    }

    return isTransitive;
  }

  template<class LeftArgExtractor, class RightArgExtractor>
  void parseTransitive(QueryTokenParseState *parserState) {
    bool isTransitive = parseTransitiveQualifier(parserState);

    if (isTransitive) {
      PQLSuchThatClauseContext::parseArgs<
          ClauseT, LeftArgExtractor, RightArgExtractor>(parserState);
    } else {
      PQLSuchThatClauseContext::parseArgs<
          Clause, LeftArgExtractor, RightArgExtractor>(parserState);
    }
  }
};
