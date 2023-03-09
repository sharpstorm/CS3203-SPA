#include "PQLWithParser.h"
#include "qps/parser/token_parser/ref_extractor/PQLAnyRefExtractor.h"

void PQLWithParser::parse(QueryTokenParseState *parserState,
                          QueryBuilder *builder) {
  parserState->expect(PQL_TOKEN_WITH);
  parseWithClause(parserState, builder);
}

void PQLWithParser::parseWithClause(QueryTokenParseState *parserState,
                                    QueryBuilder *builder) {
  // Expect either an integer, string or [syn, period, attrName]
  ClauseArgumentPtr left = PQLAnyRefExtractor::extractAttr(parserState, builder);
  // Expect an equals
  parserState->expect(PQL_TOKEN_EQUALS);
  // Expect either an integer, string or [syn, period, attrName]
  ClauseArgumentPtr right = PQLAnyRefExtractor::extractAttr(parserState, builder);
}

//PQLSynonymAttribute PQLWithParser::extractAttribute(
//    QueryTokenParseState *parserState, QueryBuilder *builder,
//    PQLSynonymType type) {
//  PQLToken* foundToken;
//  switch (type) {
//    case PQL_SYN_TYPE_VARIABLE:
//      foundToken = parserState->expect(PQL_TOKEN_VAR_NAME);
//      break;
//    case PQL_SYN_TYPE_READ:
//    case PQL_SYN_TYPE_PRINT:
//      // Read and print expect stmt#
//      foundToken = parserState->expect(PQL_TOKEN_STMT_NUM, PQL_TOKEN_VAR_NAME);
//      break;
//    case PQL_SYN_TYPE_PROCEDURE:
//      parserState->expect(PQL_TOKEN_PROC_NAME);
//    case PQL_SYN_TYPE_CALL:
//      foundToken = parserState->tryExpect(PQL_TOKEN_PROC_NAME, PQL_TOKEN_STMT_NUM);
//      break;
//    case PQL_SYN_TYPE_STMT:
//    case PQL_SYN_TYPE_ASSIGN:
//    case PQL_SYN_TYPE_IF:
//    case PQL_SYN_TYPE_WHILE:
//      foundToken = parserState->expect(PQL_TOKEN_STMT_NUM);
//      break;
//    case PQL_SYN_TYPE_CONSTANT:
//      foundToken = parserState->expect(PQL_TOKEN_VALUE);
//      break;
//  }
//
//  return getSynAttribute(foundToken);
//}
//
//PQLSynonymAttribute PQLWithParser::getSynAttribute(PQLToken * token) {
//  switch (token->getType()) {
//    case PQL_TOKEN_VAR_NAME:
//      return PQLSynonymAttribute::VAR_NAME;
//    case PQL_TOKEN_PROC_NAME:
//      return PQLSynonymAttribute::PROC_NAME;
//    case PQL_TOKEN_STMT_NUM:
//      return PQLSynonymAttribute::STMT_NUM;
//    case PQL_TOKEN_VALUE:
//      return PQLSynonymAttribute::VALUE;
//  }
//}
