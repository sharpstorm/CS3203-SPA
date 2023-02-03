#pragma once

#include "../TokenParseState.h"
#include "IPQLContext.h"
#include "../../../errors/QPSParserError.h"

const char QPS_PARSER_ERR_EOS[] = "Unexpected end of token stream";
const char QPS_PARSER_ERR_UNEXPECTED[] =
    "Unexpected token encountered during parsing";

class AbstractPQLContext: public IPQLContext {
 public:
  virtual void parse(TokenParseState* parserState) = 0;

 protected:
  template<typename... PQLTokenType>
  PQLToken *expect(TokenParseState* parserState,
                   PQLTokenType... tokenType);

  template<typename... PQLTokenCategory>
  PQLToken *expectCategory(TokenParseState* parserState,
                   PQLTokenCategory... tokenCategory);

  PQLToken *expectVarchar(TokenParseState *parserState);
};

template<typename... PQLTokenType>
PQLToken* AbstractPQLContext::expect(TokenParseState* parserState,
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
PQLToken* AbstractPQLContext::expectCategory(TokenParseState* parserState,
                                     PQLTokenCategory... tokenCategory) {
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
