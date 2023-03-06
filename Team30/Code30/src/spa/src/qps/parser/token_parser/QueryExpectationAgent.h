#pragma once

#include "../PQLToken.h"
#include "token_stream/QueryTokenStream.h"
#include "qps/errors/QPSParserSyntaxError.h"

class QueryExpectationAgent {
 private:
  QueryTokenStream* stream;
  void assertNotNull(PQLToken* token);

 public:
  explicit QueryExpectationAgent(QueryTokenStream* stream): stream(stream) {}

  template<typename... T>
  PQLToken *expect(T... tokenType);
  PQLToken *expectSynName();
};

template<typename... PQLTokenType>
PQLToken* QueryExpectationAgent::expect(PQLTokenType... tokenType) {
  PQLToken* currentToken = stream->getCurrentToken();
  assertNotNull(currentToken);

  if ((currentToken->isType(tokenType)  || ... || false)) {
    stream->advanceToken();
    return currentToken;
  }

  throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
}
