#pragma once

#include "../PQLToken.h"
#include "token_stream/QueryTokenStream.h"
#include "qps/errors/QPSParserSyntaxError.h"
#include "qps/common/PQLTypes.h"

class QueryExpectationAgent {
 private:
  QueryTokenStream* stream;
  static void assertNotNull(PQLToken* token);

 public:
  explicit QueryExpectationAgent(QueryTokenStream* stream): stream(stream) {}

  template<typename... T>
  PQLToken *expect(T... tokenType);
  template<typename... T>
  PQLToken *tryExpect(T... tokenType);
  PQLSynonymName expectSynName();
  PQLToken* expectSynAttr();
};

template<typename... PQLTokenType>
PQLToken* QueryExpectationAgent::tryExpect(PQLTokenType... tokenType) {
  PQLToken* currentToken = stream->getCurrentToken();
  if (currentToken == nullptr) {
    return nullptr;
  }

  if ((currentToken->isType(tokenType)  || ... || false)) {
    stream->advanceToken();
    return currentToken;
  }

  return nullptr;
}

template<typename... PQLTokenType>
PQLToken* QueryExpectationAgent::expect(PQLTokenType... tokenType) {
  PQLToken* currentToken = tryExpect(tokenType...);
  if (currentToken == nullptr) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
  }

  return currentToken;
}
