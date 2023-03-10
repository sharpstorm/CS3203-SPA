#include "PQLSelectParser.h"
#include "qps/parser/token_parser/ref_extractor/PQLAttributeRefExtractor.h"

const char BOOLEAN_SELECT[] = "BOOLEAN";

void PQLSelectParser::parse(QueryTokenParseState *parserState,
                            QueryBuilder *queryBuilder) {
  parserState->expect(PQL_TOKEN_SELECT);
  if (parserState->isCurrentTokenType(PQL_TOKEN_TUPLE_OPEN)) {
    parseTuple(parserState, queryBuilder);
    return;
  }

  parseSynonym(parserState, queryBuilder);
}

void PQLSelectParser::parseTuple(QueryTokenParseState *parserState,
                                 QueryBuilder *queryBuilder) {
  parserState->expect(PQL_TOKEN_TUPLE_OPEN);
  parseSynonym(parserState, queryBuilder);

  PQLToken* nextToken = parserState->expect(PQL_TOKEN_TUPLE_CLOSE,
                                            PQL_TOKEN_COMMA);
  while (!nextToken->isType(PQL_TOKEN_TUPLE_CLOSE)) {
    parseSynonym(parserState, queryBuilder);
    nextToken = parserState->expect(PQL_TOKEN_TUPLE_CLOSE, PQL_TOKEN_COMMA);
  }
}

void PQLSelectParser::addResultSynonym(QueryBuilder *queryBuilder,
                                       const string &synName) {
  PQLQuerySynonym* synonym = queryBuilder->accessSynonym(synName);
  if (synonym == nullptr) {
    return;
  }

  AttributedSynonym attrSyn(*synonym, NO_ATTRIBUTE);
  queryBuilder->addResultSynonym(attrSyn);
}

void PQLSelectParser::parseSynonym(QueryTokenParseState *parserState,
                                   QueryBuilder *queryBuilder) {
  PQLSynonymName synName = parserState->expectSynName();

  if (synName == BOOLEAN_SELECT) {
    return;
  }

  PQLQuerySynonym* syn = queryBuilder->accessSynonym(synName);
  if (parserState->isCurrentTokenType(PQL_TOKEN_PERIOD)) {
    parserState->expect(PQL_TOKEN_PERIOD);
    PQLSynonymAttribute attr =
        PQLAttributeRefExtractor::extractAttribute(parserState);
    AttributedSynonym attrSyn(*syn, attr);
    queryBuilder->addResultSynonym(attrSyn);
    return;
  }

  addResultSynonym(queryBuilder, synName);
}
