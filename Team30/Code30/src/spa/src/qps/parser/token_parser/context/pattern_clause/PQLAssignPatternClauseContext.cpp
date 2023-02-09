#include "PQLAssignPatternClauseContext.h"

#include <memory>
#include <string>
#include "../../../../clauses/AssignPatternClause.h"

using std::make_unique, std::string;

void PQLAssignPatternClauseContext::parse(QueryTokenParseState *parserState) {
  PQLToken* synonym = expectVarchar(parserState);
  PQLQueryVariable* synonymVar = parserState->getQueryBuilder()
      ->getVariable(synonym->tokenData);
  if (synonymVar == nullptr) {
    throw QPSParserError(QPS_PARSER_ERR_UNKNOWN_TOKEN);
  }
  if (!synonymVar->isType(PQL_VAR_TYPE_ASSIGN)) {
    throw QPSParserError(QPS_PARSER_ERR_PATTERN_TYPE);
  }

  expect(parserState, PQL_TOKEN_BRACKET_OPEN);
  ClauseArgument left = extractEntityRef(parserState);
  expect(parserState, PQL_TOKEN_COMMA);

  bool isWildcard = false;
  bool hasExpression = false;
  string patternString = "";
  PQLToken* nextToken = expect(parserState,
                               PQL_TOKEN_UNDERSCORE, PQL_TOKEN_QUOTE);
  if (nextToken->isType(PQL_TOKEN_UNDERSCORE)) {
    isWildcard = true;
    nextToken = expect(parserState, PQL_TOKEN_QUOTE, PQL_TOKEN_BRACKET_CLOSE);
    hasExpression = nextToken->isType(PQL_TOKEN_QUOTE);
  } else {
    hasExpression = true;
  }

  if (hasExpression) {
    nextToken = expect(parserState, PQL_TOKEN_VARIABLE, PQL_TOKEN_INTEGER);
    patternString = nextToken->tokenData;
    expect(parserState, PQL_TOKEN_QUOTE);
    if (isWildcard) {
      expect(parserState, PQL_TOKEN_UNDERSCORE);
    }
    expect(parserState, PQL_TOKEN_BRACKET_CLOSE);
  }

  parserState->getQueryBuilder()->addPattern(make_unique<AssignPatternClause>(
      *synonymVar, left, patternString, isWildcard));
}
