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
                                     PQL_TOKEN_TYPE tokenType) {
  return expectInternal(parserState, tokenType);
}

PQLToken* AbstractPQLContext::expect(TokenParseState* parserState,
                                     PQL_TOKEN_TYPE token1,
                                     PQL_TOKEN_TYPE token2) {
  return expectInternal(parserState, token1, token2);
}
