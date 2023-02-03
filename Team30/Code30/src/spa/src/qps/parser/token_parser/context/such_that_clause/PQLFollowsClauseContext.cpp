#include <utility>
#include <memory>
#include "PQLFollowsClauseContext.h"
#include "../../../../clauses/FollowsClause.h"
#include "../../../../clauses/FollowsTClause.h"

using std::unique_ptr, std::pair;

void PQLFollowsClauseContext::parse(TokenParseState *parserState) {
  PQLSuchThatClauseContext::parse(parserState);
  bool isTransitive = false;

  PQLToken* nextToken = expect(parserState,
                               PQL_TOKEN_BRACKET_OPEN,
                               PQL_TOKEN_ASTRIX);
  if (nextToken->isType(PQL_TOKEN_ASTRIX)) {
    isTransitive = true;
    expect(parserState, PQL_TOKEN_BRACKET_OPEN);
  }

  ClauseArgument left = extractStatementRef(parserState);
  expect(parserState, PQL_TOKEN_COMMA);
  ClauseArgument right = extractStatementRef(parserState);
  expect(parserState, PQL_TOKEN_BRACKET_CLOSE);

  SuchThatClause* clause;
  ClausePair args(left, right);
  if (isTransitive) {
    clause = new FollowsTClause(left, right, args);
  } else {
    clause = new FollowsClause(left, right, args);
  }

  parserState->getQueryBuilder()
    ->addSuchThat(unique_ptr<SuchThatClause>(clause));
}
