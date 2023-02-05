#include <memory>

#include "PQLUsesClauseContext.h"
#include "../../../../clauses/UsesClause.h"

using std::unique_ptr;

void PQLUsesClauseContext::parse(QueryTokenParseState *parserState) {
  PQLSuchThatClauseContext::parse(parserState);

  expect(parserState, PQL_TOKEN_BRACKET_OPEN);
  ClauseArgument left = extractAnyRef(parserState);
  expect(parserState, PQL_TOKEN_COMMA);
  ClauseArgument right = extractEntityRef(parserState);
  expect(parserState, PQL_TOKEN_BRACKET_CLOSE);

  parserState->getQueryBuilder()->addSuchThat(
      unique_ptr<SuchThatClause>(new UsesClause(left, right)));
}
