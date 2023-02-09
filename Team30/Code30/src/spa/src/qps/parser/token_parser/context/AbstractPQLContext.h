#pragma once

#include "../QueryTokenParseState.h"
#include "IPQLContext.h"
#include "../../../errors/QPSParserError.h"

const char QPS_PARSER_ERR_EOS[] = "Unexpected end of token stream";
const char QPS_PARSER_ERR_UNEXPECTED[] =
    "Unexpected token encountered during parsing";
const char QPS_PARSER_ERR_UNKNOWN_TOKEN[] = "Unknown synonym encountered";
const char QPS_PARSER_ERR_PATTERN_TYPE[] = "Invalid synonym type in pattern";

class AbstractPQLContext: public IPQLContext {
 public:
  virtual void parse(QueryTokenParseState* parserState) = 0;

 protected:
  template<typename... PQLTokenType>
  PQLToken *expect(QueryTokenParseState* parserState,
                   PQLTokenType... tokenType);

  template<typename... PQLTokenCategory>
  PQLToken *expectCategory(QueryTokenParseState* parserState,
                           PQLTokenCategory... tokenCategory);

  PQLToken *expectVarchar(QueryTokenParseState *parserState);
  ClauseArgument extractStatementRef(QueryTokenParseState* state);
  ClauseArgument extractEntityRef(QueryTokenParseState* state);
  ClauseArgument extractAnyRef(QueryTokenParseState* state);

 private:
  ClauseArgument extractCommonRef(QueryTokenParseState* state);
  ClauseArgument extractEntity(QueryTokenParseState* state);
  ClauseArgument extractStatement(QueryTokenParseState* state);
};

template<typename... PQLTokenType>
PQLToken* AbstractPQLContext::expect(QueryTokenParseState* parserState,
                                     PQLTokenType... tokenType) {
  PQLToken* currentToken = parserState->getCurrentToken();

  if (currentToken == nullptr) {
    throw QPSParserError(QPS_PARSER_ERR_EOS);
  }

  if ((currentToken->isType(tokenType)  || ... || false)) {
    parserState->advanceToken();
    return currentToken;
  }

  throw QPSParserError(QPS_PARSER_ERR_UNEXPECTED);
}

template<typename... PQLTokenCategory>
PQLToken* AbstractPQLContext::expectCategory(
    QueryTokenParseState* parserState, PQLTokenCategory... tokenCategory) {
  PQLToken* currentToken = parserState->getCurrentToken();

  if (currentToken == nullptr) {
    throw QPSParserError(QPS_PARSER_ERR_EOS);
  }

  if ((currentToken->isCategory(tokenCategory)  || ... || false)) {
    parserState->advanceToken();
    return currentToken;
  }

  throw QPSParserError(QPS_PARSER_ERR_UNEXPECTED);
}
