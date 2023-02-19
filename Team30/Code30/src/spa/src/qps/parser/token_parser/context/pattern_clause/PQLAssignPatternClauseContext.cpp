#include "PQLAssignPatternClauseContext.h"

#include <memory>
#include <string>
#include <utility>
#include "qps/clauses/pattern/AssignPatternClause.h"
#include "qps/parser/token_parser/ref_extractor/PQLEntityRefExtractor.h"

using std::make_unique, std::string, std::unique_ptr, std::move;

void PQLAssignPatternClauseContext::parse(QueryTokenParseState *parserState) {
  parserState->expect(PQL_TOKEN_BRACKET_OPEN);
  ClauseArgumentPtr left = PQLEntityRefExtractor::extract(parserState);
  parserState->expect(PQL_TOKEN_COMMA);
  PQLAssignPatternClauseContext::PatternData patternData =
      extractPatternData(parserState);

  parserState->getQueryBuilder()->addPattern(make_unique<AssignPatternClause>(
      *synonym, move(left), patternData.pattern, patternData.isPartial));
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
