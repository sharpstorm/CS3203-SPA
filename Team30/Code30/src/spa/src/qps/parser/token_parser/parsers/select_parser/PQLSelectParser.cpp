#include "PQLSelectParser.h"
#include "qps/parser/token_parser/ref_extractor/PQLAttributeRefExtractor.h"
#include "qps/errors/QPSParserSemanticError.h"
#include "qps/parser/common/QueryKeywords.h"

void PQLSelectParser::parse(QueryTokenParseState *parserState,
                            QueryBuilder *queryBuilder) {
  parserState->expect(PQL_TOKEN_SELECT);
  if (parserState->isCurrentTokenType(PQL_TOKEN_TUPLE_OPEN)) {
    parseTuple(parserState, queryBuilder);
    return;
  }

  if (parserState->isCurrentTokenType(PQL_TOKEN_BOOLEAN)
      && !queryBuilder->hasSynonym(PQL_KEYWORD_BOOLEAN)) {
    parserState->advanceToken();
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

void PQLSelectParser::parseSynonym(QueryTokenParseState *parserState,
                                   QueryBuilder *queryBuilder) {
  AttributedSynonymPtr attrSyn =
      PQLAttributeRefExtractor::extract(parserState, queryBuilder);
  if (attrSyn == nullptr) {
    return;
  }

  if (!attrSyn->validateAttribute()) {
    queryBuilder->setError(QPS_PARSER_ERR_INVALID_ATTRIBUTE);
    return;
  }

  queryBuilder->addResultSynonym(*attrSyn);
}
