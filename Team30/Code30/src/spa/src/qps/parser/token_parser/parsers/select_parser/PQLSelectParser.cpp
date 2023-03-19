#include "PQLSelectParser.h"
#include "qps/parser/token_parser/ref_extractor/PQLAttributeRefExtractor.h"
#include "qps/errors/QPSParserSemanticError.h"

const char BOOLEAN_SELECT[] = "BOOLEAN";

void PQLSelectParser::parse(QueryTokenParseState *parserState,
                            QueryBuilder *queryBuilder) {
  parserState->expect(PQL_TOKEN_SELECT);
  if (parserState->isCurrentTokenType(PQL_TOKEN_TUPLE_OPEN)) {
    parseTuple(parserState, queryBuilder);
    return;
  }

  PQLSynonymName synName = parserState->expectSynName();

  if (synName == BOOLEAN_SELECT && !queryBuilder->hasSynonym(synName)) {
    return;
  }

  parseSynonym(parserState, queryBuilder, synName);
}

void PQLSelectParser::parseTuple(QueryTokenParseState *parserState,
                                 QueryBuilder *queryBuilder) {
  parserState->expect(PQL_TOKEN_TUPLE_OPEN);
  PQLSynonymName synName = parserState->expectSynName();
  parseSynonym(parserState, queryBuilder, synName);

  PQLToken* nextToken = parserState->expect(PQL_TOKEN_TUPLE_CLOSE,
                                            PQL_TOKEN_COMMA);
  while (!nextToken->isType(PQL_TOKEN_TUPLE_CLOSE)) {
    synName = parserState->expectSynName();
    parseSynonym(parserState, queryBuilder, synName);
    nextToken = parserState->expect(PQL_TOKEN_TUPLE_CLOSE, PQL_TOKEN_COMMA);
  }
}

void PQLSelectParser::addResultSynonym(QueryBuilder *queryBuilder,
                                       const string &synName) {
  PQLQuerySynonymProxy* synProxy = queryBuilder->accessSynonym(synName);
  if (synProxy == nullptr) {
    return;
  }

  AttributedSynonym attrSyn(*synProxy, NO_ATTRIBUTE);
  queryBuilder->addResultSynonym(attrSyn);
}

void PQLSelectParser::parseSynonym(QueryTokenParseState *parserState,
                                   QueryBuilder *queryBuilder,
                                   const string &synName) {
  PQLQuerySynonymProxy* syn = queryBuilder->accessSynonym(synName);
  if (syn == nullptr) {
    return;
  }
  PQLQuerySynonymProxy synValue = *syn;

  if (parserState->isCurrentTokenType(PQL_TOKEN_PERIOD)) {
    parserState->expect(PQL_TOKEN_PERIOD);
    PQLSynonymAttribute attr =
        PQLAttributeRefExtractor::extractAttribute(parserState);
    if (!isValidAttribute(synValue->getType(),attr)) {
      throw QPSParserSemanticError(QPS_PARSER_ERR_INVALID_ATTRIBUTE);
    }

    AttributedSynonym attrSyn(*syn, attr);
    queryBuilder->addResultSynonym(attrSyn);
    return;
  }

  addResultSynonym(queryBuilder, synName);
}
bool PQLSelectParser::isValidAttribute(PQLSynonymType synType,
                                       PQLSynonymAttribute attrType) {
  switch (synType) {
    case PQL_SYN_TYPE_STMT:
    case PQL_SYN_TYPE_WHILE:
    case PQL_SYN_TYPE_IF:
    case PQL_SYN_TYPE_ASSIGN:
      return attrType == STMT_NUM;
    case PQL_SYN_TYPE_VARIABLE:
      return attrType == VAR_NAME;
    case PQL_SYN_TYPE_CONSTANT:
      return attrType == CONST_VALUE;
    case PQL_SYN_TYPE_PROCEDURE:
      return attrType == PROC_NAME;
    case PQL_SYN_TYPE_READ:
    case PQL_SYN_TYPE_PRINT:
      return attrType == STMT_NUM || attrType == VAR_NAME;
    case PQL_SYN_TYPE_CALL:
      return attrType == STMT_NUM || attrType == PROC_NAME;
  }
}
