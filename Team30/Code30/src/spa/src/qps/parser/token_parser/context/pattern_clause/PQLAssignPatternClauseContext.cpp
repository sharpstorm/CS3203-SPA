#include "PQLAssignPatternClauseContext.h"

#include <memory>
#include <string>
#include "../../../../clauses/AssignPatternClause.h"
#include "qps/errors/QPSParserSemanticError.h"
#include "qps/parser/token_parser/ref_extractor/PQLEntityRefExtractor.h"

using std::make_unique, std::string;

void PQLAssignPatternClauseContext::parse(QueryTokenParseState *parserState) {
  parserState->advanceStage(TOKEN_PARSE_STAGE_PATTERN_MARKER);
  parserState->advanceStage(TOKEN_PARSE_STAGE_PATTERN);
  PQLToken* synonym = parserState->expectSynName();
  parserState->expect(PQL_TOKEN_BRACKET_OPEN);
  ClauseArgument left = PQLEntityRefExtractor::extract(parserState);
  parserState->expect(PQL_TOKEN_COMMA);

  PQLAssignPatternClauseContext::PatternData patternData =
      extractPatternData(parserState);

  PQLQuerySynonym* synonymVar = parserState->getQueryBuilder()
      ->getVariable(synonym->getData());
  if (synonymVar == nullptr) {
    parserState->setSemanticError(QPS_PARSER_ERR_UNKNOWN_SYNONYM);
    return;
  }

  if (!synonymVar->isType(PQL_SYN_TYPE_ASSIGN)) {
    parserState->setSemanticError(QPS_PARSER_ERR_PATTERN_TYPE);
    return;
  }

  parserState->getQueryBuilder()->addPattern(make_unique<AssignPatternClause>(
      *synonymVar, left, patternData.pattern, patternData.isPartial));
}

PQLAssignPatternClauseContext::PatternData
PQLAssignPatternClauseContext::extractPatternData(
    QueryTokenParseState *parserState) {
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
    patternString = nextToken->getData();
    parserState->expect(PQL_TOKEN_QUOTE);
    if (isWildcard) {
      parserState->expect(PQL_TOKEN_UNDERSCORE);
    }
    parserState->expect(PQL_TOKEN_BRACKET_CLOSE);
  }

  return PQLAssignPatternClauseContext::PatternData{
    isWildcard,
    patternString
  };
}
