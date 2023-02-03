#include <memory>
#include "PQLModifiesClauseContext.h"
#include "../../../../clauses/ModifiesClause.h"

using std::unique_ptr;

void PQLModifiesClauseContext::parse(TokenParseState *parserState) {
  PQLSuchThatClauseContext::parse(parserState);

  expect(parserState, PQL_TOKEN_BRACKET_OPEN);
  ClauseArgument left = extractAnyRef(parserState);
  expect(parserState, PQL_TOKEN_COMMA);
  ClauseArgument right = extractEntityRef(parserState);
  expect(parserState, PQL_TOKEN_BRACKET_CLOSE);

  parserState->getQueryBuilder()->addSuchThat(
      unique_ptr<SuchThatClause>(new ModifiesClause(left, right)));
}
