#pragma once

#include <memory>
#include <utility>

#include "qps/parser/PQLToken.h"
#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/parser/token_parser/QueryTokenParseState.h"
#include "qps/clauses/SuchThatClause.h"
#include "qps/parser/builder/QueryBuilder.h"
#include "IPQLSuchThatClauseContext.h"

using std::unique_ptr, std::make_unique, std::move;


class AbstractSuchThatClauseContext: public IPQLSuchThatClauseContext {
 protected:
  template<class Clause, class LeftArgExtractor, class RightArgExtractor>
  SuchThatClausePtr parseArgs(QueryTokenParseState *parserState,
                          QueryBuilder* builder);
};

template<class Clause, class LeftArgExtractor, class RightArgExtractor>
SuchThatClausePtr AbstractSuchThatClauseContext::parseArgs(
    QueryTokenParseState *parserState,
    QueryBuilder* builder) {
  parserState->expect(PQL_TOKEN_BRACKET_OPEN);
  ClauseArgumentPtr left = LeftArgExtractor::extract(parserState, builder);
  parserState->expect(PQL_TOKEN_COMMA);
  ClauseArgumentPtr right = RightArgExtractor::extract(parserState, builder);
  parserState->expect(PQL_TOKEN_BRACKET_CLOSE);

  if (left == nullptr || right == nullptr) {
    return nullptr;
  }

  return make_unique<Clause>(std::move(left), std::move(right));
}
