#include "PQLAssignPatternClauseContext.h"

#include <memory>
#include <string>
#include "../../../../clauses/AssignPatternClause.h"

using std::make_unique, std::string;

void PQLAssignPatternClauseContext::parse(QueryTokenParseState *parserState) {
  parserState->advanceStage(TOKEN_PARSE_STAGE_PATTERN_MARKER);

  PQLToken* synonym = expectVarchar(parserState);
  PQLQuerySynonym* synonymVar = parserState->getQueryBuilder()
      ->getVariable(synonym->tokenData);
  if (synonymVar == nullptr) {
    throw QPSParserError(QPS_PARSER_ERR_UNKNOWN_TOKEN);
  }
  if (!synonymVar->isType(PQL_SYN_TYPE_ASSIGN)) {
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
    nextToken = expectVarchar(parserState);
    patternString = nextToken->tokenData;
    expect(parserState, PQL_TOKEN_QUOTE);
    if (isWildcard) {
      expect(parserState, PQL_TOKEN_UNDERSCORE);
    }
    expect(parserState, PQL_TOKEN_BRACKET_CLOSE);
  }
  parserState->advanceStage(TOKEN_PARSE_STAGE_PATTERN);
  parserState->getQueryBuilder()->addPattern(make_unique<AssignPatternClause>(
      *synonymVar, left, patternString, isWildcard));
}
