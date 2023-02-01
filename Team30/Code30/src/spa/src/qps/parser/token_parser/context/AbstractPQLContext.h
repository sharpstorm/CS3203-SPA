#pragma once

#include "../TokenParseState.h"
#include "IPQLContext.h"

class AbstractPQLContext: public IPQLContext {
 public:
  virtual void parse(TokenParseState* parserState) = 0;

 protected:
  PQLToken* expect(TokenParseState* parserState, PQL_TOKEN_TYPE tokenType);
  PQLToken* expect(TokenParseState* parserState, PQL_TOKEN_TYPE token1,
                   PQL_TOKEN_TYPE token2);

 private:
  template<typename... PQL_TOKEN_TYPE>
  PQLToken* expectInternal(TokenParseState* parserState,
                  PQL_TOKEN_TYPE... tokenType);
};
