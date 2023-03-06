#pragma once

#include "AbstractSuchThatClauseContext.h"

template<class Clause, class LeftArgExtractor, class RightArgExtractor>
class PQLSuchThatClauseContext : public AbstractSuchThatClauseContext {
 public:
  SuchThatClausePtr parse(QueryTokenParseState *parserState,
                          QueryBuilder* builder) {
    return AbstractSuchThatClauseContext::parseArgs<
        Clause, LeftArgExtractor, RightArgExtractor>(parserState, builder);
  }
};
