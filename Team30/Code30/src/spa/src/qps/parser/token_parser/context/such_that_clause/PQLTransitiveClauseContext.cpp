#include "PQLTransitiveClauseContext.h"

bool PQLTransitiveClauseContext::parseTransitiveQualifier(
    QueryTokenParseState *parserState) {
  PQLToken* nextToken = expect(parserState,
                               PQL_TOKEN_BRACKET_OPEN,
                               PQL_TOKEN_ASTRIX);

  if (nextToken->isType(PQL_TOKEN_ASTRIX)) {
    expect(parserState, PQL_TOKEN_BRACKET_OPEN);
    return true;
  }

  return false;
}
