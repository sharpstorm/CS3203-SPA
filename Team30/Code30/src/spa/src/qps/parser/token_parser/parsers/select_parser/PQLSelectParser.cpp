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

  PQLSynonymName synName = parserState->expectSynName();
  if (synName == BOOLEAN_SELECT) {
    return;
  }

  // TODO(KwanHW): To test
  PQLQuerySynonym* syn = queryBuilder->accessSynonym(synName);
  if (parserState->tryExpect(PQL_TOKEN_PERIOD)) {
    PQLSynonymAttribute attr =
        PQLAttributeRefExtractor::extractAttribute(parserState, syn->getType());
    AttributedSynonym attrSyn(*syn, attr);
    queryBuilder->addResultSynonym(attrSyn);
    return;
  }

  addResultSynonym(queryBuilder, synName);
}

void PQLSelectParser::parseTuple(QueryTokenParseState *parserState,
                                 QueryBuilder *queryBuilder) {
  parserState->expect(PQL_TOKEN_TUPLE_OPEN);
  PQLSynonymName synName = parserState->expectSynName();
  addResultSynonym(queryBuilder, synName);

  PQLToken* nextToken = parserState->expect(PQL_TOKEN_TUPLE_CLOSE,
                                            PQL_TOKEN_COMMA);
  while (!nextToken->isType(PQL_TOKEN_TUPLE_CLOSE)) {
    synName = parserState->expectSynName();
    PQLQuerySynonym* syn = queryBuilder->accessSynonym(synName);

    if (parserState->tryExpect(PQL_TOKEN_PERIOD)) {
      PQLSynonymAttribute attr =
          PQLAttributeRefExtractor::extractAttribute(parserState, syn->getType());
      AttributedSynonym attrSyn(*syn, attr);
      queryBuilder->addResultSynonym(attrSyn);
      continue;
    }

    addResultSynonym(queryBuilder, synName);
    nextToken = parserState->expect(PQL_TOKEN_TUPLE_CLOSE,
                                    PQL_TOKEN_COMMA);
  }
}

void PQLSelectParser::addResultSynonym(QueryBuilder *queryBuilder,
                                       const string &synName) {
  PQLQuerySynonym* synonym = queryBuilder->accessSynonym(synName);
  if (synonym == nullptr) {
    return;
  }

  queryBuilder->addResultSynonym(*synonym);
}
