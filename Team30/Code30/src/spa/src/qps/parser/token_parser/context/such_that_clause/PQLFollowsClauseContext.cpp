#include <memory>
#include "PQLFollowsClauseContext.h"
#include "../../../../clauses/FollowsClause.h"
#include "../../../../clauses/FollowsTClause.h"

using std::unique_ptr;

void PQLFollowsClauseContext::parse(QueryTokenParseState *parserState) {
  PQLSuchThatClauseContext::parse(parserState);
  bool isTransitive = parseTransitiveQualifier(parserState);

  ClauseArgument left = extractStatementRef(parserState);
  expect(parserState, PQL_TOKEN_COMMA);
  ClauseArgument right = extractStatementRef(parserState);
  expect(parserState, PQL_TOKEN_BRACKET_CLOSE);

  SuchThatClause* clause;
  if (isTransitive) {
    clause = new FollowsTClause(left, right);
  } else {
    clause = new FollowsClause(left, right);
  }

  parserState->getQueryBuilder()
    ->addSuchThat(unique_ptr<SuchThatClause>(clause));
}
