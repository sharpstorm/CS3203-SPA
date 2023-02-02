#pragma once

#include "../TokenParseState.h"
#include "IPQLContext.h"

class AbstractPQLContext: public IPQLContext {
 public:
  virtual void parse(TokenParseState* parserState) = 0;

 protected:
  PQLToken* expect(TokenParseState* parserState, PQLTokenType tokenType);
  PQLToken* expect(TokenParseState* parserState, PQLTokenType token1,
                   PQLTokenType token2);

 private:
  template<typename... PQLTokenType>
  PQLToken* expectInternal(TokenParseState* parserState,
                           PQLTokenType... tokenType);
};
