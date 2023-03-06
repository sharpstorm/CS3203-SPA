#pragma once

#include "AbstractSuchThatClauseContext.h"

template<class Clause, class ClauseT, class LeftArgExtractor,
    class RightArgExtractor>
class PQLTransitiveClauseContext : public AbstractSuchThatClauseContext {
 public:
  SuchThatClausePtr parse(QueryTokenParseState *parserState,
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

 private:
  bool parseTransitiveQualifier(QueryTokenParseState *parserState) {
    return parserState->tryExpect(PQL_TOKEN_ASTRIX) != nullptr;
  }
};
