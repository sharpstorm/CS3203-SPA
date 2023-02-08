#include <memory>
#include "PQLParentClauseContext.h"
#include "../../../../clauses/ParentClause.h"
#include "../../../../clauses/ParentTClause.h"

using std::unique_ptr;

void PQLParentClauseContext::parse(QueryTokenParseState *parserState) {
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
  if (isTransitive) {
    clause = new ParentTClause(left, right);
  } else {
    clause = new ParentClause(left, right);
  }

  parserState->getQueryBuilder()
    ->addSuchThat(unique_ptr<SuchThatClause>(clause));
}