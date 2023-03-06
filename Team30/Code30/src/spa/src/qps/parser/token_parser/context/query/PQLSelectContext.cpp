#include "PQLSelectContext.h"

const char BOOLEAN_SELECT[] = "BOOLEAN";

void PQLSelectContext::parse(QueryTokenParseState *parserState) {
  parserState->advanceStage(TOKEN_PARSE_STAGE_COMMAND);

  if (parserState->isCurrentTokenType(PQL_TOKEN_TUPLE_OPEN)) {
    parseTuple(parserState);
    return;
  }

  PQLSynonymName synName = parserState->expectSynName()->getData();
  if (synName == BOOLEAN_SELECT) {
    return;
  }

  addResultSynonym(parserState, synName);
}

void PQLSelectContext::parseTuple(QueryTokenParseState *parserState) {
  parserState->expect(PQL_TOKEN_TUPLE_OPEN);
  PQLSynonymName synName = parserState->expectSynName()->getData();
  addResultSynonym(parserState, synName);

  PQLToken* nextToken = parserState->expect(PQL_TOKEN_TUPLE_CLOSE,
                                            PQL_TOKEN_COMMA);
  while (!nextToken->isType(PQL_TOKEN_TUPLE_CLOSE)) {
    synName = parserState->expectSynName()->getData();
    addResultSynonym(parserState, synName);
    nextToken = parserState->expect(PQL_TOKEN_TUPLE_CLOSE,
                                    PQL_TOKEN_COMMA);
  }
}

void PQLSelectContext::addResultSynonym(QueryTokenParseState *parserState,
                                    string synName) {
  PQLQuerySynonym* synonym = parserState->getQueryBuilder()
      ->accessSynonym(synName);
  if (synonym == nullptr) {
    return;
  }

  parserState->getQueryBuilder()->addResultSynonym(*synonym);
}
