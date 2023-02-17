#include "PQLPatternContext.h"
#include "qps/errors/QPSParserSyntaxError.h"
#include "PQLAssignPatternClauseContext.h"

void PQLPatternContext::parse(QueryTokenParseState *parserState) {
  parserState->advanceStage(TOKEN_PARSE_STAGE_PATTERN_MARKER);
  parsePatternClause(parserState);
}

void PQLPatternContext::parsePatternClause(QueryTokenParseState *parserState) {
  parserState->advanceStage(TOKEN_PARSE_STAGE_PATTERN);
  PQLQuerySynonym* synonym = parseSynonym(parserState);
  dispatchPatternContext(synonym, parserState);
}

PQLQuerySynonym* PQLPatternContext::parseSynonym(
    QueryTokenParseState *parserState) {
  PQLSynonymName synName = parserState->expectSynName()->getData();
  PQLQuerySynonym* synonymVar = parserState->getQueryBuilder()
      ->accessSynonym(synName);

  if (synonymVar == nullptr) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_PATTERN_TYPE);
  }

  return synonymVar;
}

void PQLPatternContext::dispatchPatternContext(
    PQLQuerySynonym* synonym,
    QueryTokenParseState *parserState) {
  switch (synonym->getType()) {
    case PQL_SYN_TYPE_ASSIGN:
      PQLAssignPatternClauseContext(synonym).parse(parserState);
      break;
    default:
      throw QPSParserSyntaxError(QPS_PARSER_ERR_PATTERN_TYPE);
  }
}
