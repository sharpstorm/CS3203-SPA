#include "PQLAssignPatternClauseContext.h"

#include <memory>
#include <string>
#include "../../../../clauses/AssignPatternClause.h"
#include "qps/parser/token_parser/ref_extractor/PQLEntityRefExtractor.h"

using std::make_unique, std::string;

void PQLAssignPatternClauseContext::parse(QueryTokenParseState *parserState) {
  parserState->advanceStage(TOKEN_PARSE_STAGE_PATTERN_MARKER);

  PQLToken* synonym = parserState->expectVarchar();
  PQLQuerySynonym* synonymVar = parserState->getQueryBuilder()
      ->getVariable(synonym->tokenData);
  if (synonymVar == nullptr) {
    throw QPSParserError(QPS_PARSER_ERR_UNKNOWN_TOKEN);
  }
  if (!synonymVar->isType(PQL_SYN_TYPE_ASSIGN)) {
    throw QPSParserError(QPS_PARSER_ERR_PATTERN_TYPE);
  }

  parserState->expect(PQL_TOKEN_BRACKET_OPEN);
  ClauseArgument left = PQLEntityRefExtractor::extract(parserState);
  parserState->expect(PQL_TOKEN_COMMA);

  bool isWildcard = false;
  bool hasExpression = false;
  string patternString = "";
  PQLToken* nextToken = parserState->expect(
      PQL_TOKEN_UNDERSCORE, PQL_TOKEN_QUOTE);
  if (nextToken->isType(PQL_TOKEN_UNDERSCORE)) {
    isWildcard = true;
    nextToken = parserState->expect(PQL_TOKEN_QUOTE, PQL_TOKEN_BRACKET_CLOSE);
    hasExpression = nextToken->isType(PQL_TOKEN_QUOTE);
  } else {
    hasExpression = true;
  }

  if (hasExpression) {
    nextToken = parserState->expectVarchar();
    patternString = nextToken->tokenData;
    parserState->expect(PQL_TOKEN_QUOTE);
    if (isWildcard) {
      parserState->expect(PQL_TOKEN_UNDERSCORE);
    }
    parserState->expect(PQL_TOKEN_BRACKET_CLOSE);
  }
  parserState->advanceStage(TOKEN_PARSE_STAGE_PATTERN);
  parserState->getQueryBuilder()->addPattern(make_unique<AssignPatternClause>(
      *synonymVar, left, patternString, isWildcard));
}
