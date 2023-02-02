#include "AbstractPQLContext.h"
#include "../../../errors//QPSParserError.h"

template<typename... PQL_TOKEN_TYPE>
PQLToken* AbstractPQLContext::expectInternal(TokenParseState* parserState,
                                    PQL_TOKEN_TYPE... tokenType) {
  PQLToken* currentToken = parserState->getCurrentToken();

  if (currentToken == nullptr) {
    throw QPSParserError("Unexpected end of token stream");
  }

  if ((currentToken->isType(tokenType)  || ... || false)) {
    parserState->advanceToken();
    return currentToken;
  }
  throw QPSParserError("Unexpected token encountered during parsing");
}

PQLToken* AbstractPQLContext::expect(TokenParseState* parserState,
                                     PQLTokenType tokenType) {
  return expectInternal(parserState, tokenType);
}

PQLToken* AbstractPQLContext::expect(TokenParseState* parserState,
                                     PQLTokenType token1,
                                     PQLTokenType token2) {
  return expectInternal(parserState, token1, token2);
}
